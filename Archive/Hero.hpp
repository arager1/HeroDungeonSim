#ifndef INCLUE_GUARD_HERO_HPP
#define INCLUE_GUARD_HERO_HPP

#include <string>
#include "json/include/nlohmann/json.hpp"
#include "Utils.hpp"

class StatusBar
{
public:
    StatusBar(int max) : m_max{max}, m_cur{m_max} {}

    StatusBar(int max, int cur) : m_max{max}, m_cur{cur} {}

    void damage(int amt) {m_cur -= amt;}
    void damageToZero(int amt) {m_cur -= (amt < m_cur) ? amt : m_cur;}
    void heal(int amt) {m_cur += amt;}
    void healToMax(int amt) {m_cur += (amt < (m_max - m_cur)) ? amt : (m_max - m_cur);}

    void reset() {m_cur = m_max;}

    void increaseMax(int amt) {m_max += amt;}
    void decreaseMax(int amt) {m_max -= (amt < m_max) ? amt : m_max;}

    int current() const {return m_cur;}
    int max() const {return m_max;}

    bool isBelowZero() const {return m_cur < 0;}
    bool isFull() const {return m_cur >= m_max;}

private:
    int m_max;
    int m_cur;
};


struct HeroDefenseStatusBar
{
    HeroDefenseStatusBar(int resistStat, int statusMax) : m_resistanceStat{resistStat}, m_statusBar{StatusBar{statusMax}} {}

    int m_resistanceStat;
    StatusBar m_statusBar;
    int m_medicineQty;
};


struct HeroDefenseStats
{
    HeroDefenseStats(const nlohmann::json& fileData) :
        m_resiliancy {fileData.at("resilliancy")},
        m_overallHP {fileData.at("constitutions").at("overall")},
        m_bludgeonHP {HeroDefenseStatusBar{fileData.at("resistances").at("bludgeon"), fileData.at("constitutions").at("bludgeon")}},
        m_poisonHP {HeroDefenseStatusBar{fileData.at("resistances").at("poison"), fileData.at("constitutions").at("poison")}},
        m_shockHP {HeroDefenseStatusBar{fileData.at("resistances").at("shock"), fileData.at("constitutions").at("shock")}},
        m_weaknessHP {HeroDefenseStatusBar{fileData.at("resistances").at("weakness"), fileData.at("constitutions").at("weakness")}},
        m_bleedHP {HeroDefenseStatusBar{fileData.at("resistances").at("bleed"), fileData.at("constitutions").at("bleed")}},
        m_corrosiveHP {HeroDefenseStatusBar{fileData.at("resistances").at("corrosive"), fileData.at("constitutions").at("corrosive")}},
        m_burnHP {HeroDefenseStatusBar{fileData.at("resistances").at("burn"), fileData.at("constitutions").at("burn")}},
        m_frostbiteHP {HeroDefenseStatusBar{fileData.at("resistances").at("frostbite"), fileData.at("constitutions").at("frostbite")}},
        m_psychicHP {HeroDefenseStatusBar{fileData.at("resistances").at("psychic"), fileData.at("constitutions").at("psychic")}} {}

    int m_resiliancy;
    StatusBar m_overallHP;

    HeroDefenseStatusBar m_bludgeonHP;
    HeroDefenseStatusBar m_poisonHP;
    HeroDefenseStatusBar m_shockHP;
    HeroDefenseStatusBar m_weaknessHP;
    HeroDefenseStatusBar m_bleedHP;
    HeroDefenseStatusBar m_corrosiveHP;
    HeroDefenseStatusBar m_burnHP;
    HeroDefenseStatusBar m_frostbiteHP;
    HeroDefenseStatusBar m_psychicHP;
};


struct HeroOffenseStats
{
    HeroOffenseStats(const nlohmann::json& fileData) : 
        m_willpower {fileData.at("willpower")},
        m_strengthProficiency {fileData.at("proficiencies").at("strength")},
        m_agilityProficiency {fileData.at("proficiencies").at("agility")},
        m_intellegenceProficiency {fileData.at("proficiencies").at("intellegence")} {}

    int m_willpower;
    int m_strengthProficiency;
    int m_agilityProficiency;
    int m_intellegenceProficiency;
};


class HeroMemory
{
public:
    int maxDamageSeen() const {return m_maxDamageSeen;}
    int totalDamageRecieved() const {return m_totalDamageRecieved;}

    void rememberNewDamage(int newDamage)
    {
        m_totalDamageRecieved += newDamage;
        if (newDamage > m_maxDamageSeen) m_maxDamageSeen = newDamage;

        // More advanced things
    }

private:
    int m_maxDamageSeen{};
    int m_totalDamageRecieved{};
};


class Hero
{
public:
    Hero(const nlohmann::json& fileData) :
        m_name {fileData.at("name")},
        m_defenseStats {fileData.at("stats")},
        m_offenseStats {fileData.at("stats")},
        m_memory{} {}
    
    void damage(int damageAmount, DamageType type)
    {
        const int dmg {damageAmount - static_cast<int>(static_cast<float>(damageAmount) *
                                      static_cast<float>(resistance(type)) * 0.01)};

        modifyOverallHP().damage(dmg);
        modifyTypedHP(type).m_statusBar.damage(dmg);

        m_memory.rememberNewDamage(dmg);

        Logger::logDamage(dmg, type, currentOverallHP(), overallConstitution(), 
                          currentTypedHP(type), typedConstitution(type));
        Logger::logHP("Overall", currentOverallHP(), overallConstitution());
    }

    int proficiencyCheck(int strengthCheck, int agilityCheck, int intellegenceCheck) const
    {
        return (proficiencyCheckHelper(strengthCheck, ProficiencyType::Strength) ? 1 : 0) +
               (proficiencyCheckHelper(agilityCheck, ProficiencyType::Agility) ? 1 : 0) +
               (proficiencyCheckHelper(intellegenceCheck, ProficiencyType::Intellegence) ? 1 : 0);
    }

    bool willingToAttempt() const
    {
        return currentOverallHP() > m_memory.maxDamageSeen();
    }

    int resilliancy() const {return m_defenseStats.m_resiliancy;}

    int resistance(DamageType type) const {return getRefToTypedHP(type).m_resistanceStat;}

    int currentOverallHP() const {return m_defenseStats.m_overallHP.current();}

    int currentTypedHP(DamageType type) const {return getRefToTypedHP(type).m_statusBar.current();}

    int overallConstitution() const {return m_defenseStats.m_overallHP.max();}

    int typedConstitution(DamageType type) const {return getRefToTypedHP(type).m_statusBar.max();}

    int willpower() const {return m_offenseStats.m_willpower;}

    int proficiency(ProficiencyType type) const
    {
        switch (type)
        {
        case ProficiencyType::Strength: return m_offenseStats.m_strengthProficiency;
        case ProficiencyType::Agility: return m_offenseStats.m_agilityProficiency;
        case ProficiencyType::Intellegence: return m_offenseStats.m_intellegenceProficiency;
        default: return 0;
        }
    }

    bool isAlive() const {return currentOverallHP() > 0;}

    std::string name() const {return m_name;}

private:
    std::string m_name{};
    HeroDefenseStats m_defenseStats;
    HeroOffenseStats m_offenseStats;
    HeroMemory m_memory;

    StatusBar& modifyOverallHP() {return m_defenseStats.m_overallHP;}

    HeroDefenseStatusBar& modifyTypedHP(DamageType type)
    {
        switch (type)
        {
        case DamageType::Bludgeon: return m_defenseStats.m_bludgeonHP;
        case DamageType::Poison: return m_defenseStats.m_poisonHP;
        case DamageType::Shock: return m_defenseStats.m_shockHP;
        case DamageType::Weakness: return m_defenseStats.m_weaknessHP;
        case DamageType::Bleed: return m_defenseStats.m_bleedHP;
        case DamageType::Corrosive: return m_defenseStats.m_corrosiveHP;
        case DamageType::Burn: return m_defenseStats.m_burnHP;
        case DamageType::Frostbite: return m_defenseStats.m_frostbiteHP;
        case DamageType::Psychic: return m_defenseStats.m_psychicHP;
        default: return m_defenseStats.m_psychicHP;
        }
    }

    const HeroDefenseStatusBar& getRefToTypedHP(DamageType type) const
    {
        switch (type)
        {
        case DamageType::Bludgeon: return m_defenseStats.m_bludgeonHP;
        case DamageType::Poison: return m_defenseStats.m_poisonHP;
        case DamageType::Shock: return m_defenseStats.m_shockHP;
        case DamageType::Weakness: return m_defenseStats.m_weaknessHP;
        case DamageType::Bleed: return m_defenseStats.m_bleedHP;
        case DamageType::Corrosive: return m_defenseStats.m_corrosiveHP;
        case DamageType::Burn: return m_defenseStats.m_burnHP;
        case DamageType::Frostbite: return m_defenseStats.m_frostbiteHP;
        case DamageType::Psychic: return m_defenseStats.m_psychicHP;
        default: return m_defenseStats.m_psychicHP;
        }
    }

    bool proficiencyCheckHelper(int baseClearChance, ProficiencyType type) const
    {
        const bool willpowerCheck {rngCheckPercent(willpower())};
        const bool proficiencyCheck {rngCheckPercent(baseClearChance + proficiency(type))};

        Logger::log(getProficiencyTypeName(type) + " check: Willpower " + (willpowerCheck ? "Success!" : "FAILED") + " Proficiency " + (proficiencyCheck ? "Success!" : "FAILED"));

        return willpowerCheck && proficiencyCheck;
    }
};

#endif

#ifndef INCLUE_GUARD_HERO_HPP
#define INCLUE_GUARD_HERO_HPP

#include <string>
#include "StatInterfaces.hpp"

class StatusBar
{
public:
    StatusBar(int max) : m_max{max}, m_cur{m_max} {}

    void damage(int amt) {m_cur -= amt;}
    void damageToZero(int amt) {m_cur -= (amt < m_cur) ? amt : m_cur;}
    void heal(int amt) {m_cur += amt;}
    void healToMax(int amt) {m_cur += (amt < (m_max - m_cur)) ? amt : (m_max - m_cur);}

    void reset() {m_cur = m_max;}

    void increaseMax(int amt) {m_max += amt;}
    void decreaseMax(int amt) {m_max -= (amt < m_max) ? amt : m_max;}
    void setMax(int newMax) { m_max = newMax; }

    int current() const {return m_cur;}
    int max() const {return m_max;}

    bool isEmpty() const {return m_cur <= 0;}
    bool isFull() const {return m_cur >= m_max;}

private:
    int m_max;
    int m_cur;
};


class HeroMedicineStatus
{
public:
    bool isApplied() const { return m_bonusValue != 0; }

    int currentBonus() const { return m_bonusValue; }

    void applyMedicine(int potencySum)
    {
        if (potencySum <= 3) { m_bonusValue = 1; m_medicineTimer.set(1); }
        else if (potencySum == 4) { m_bonusValue = 1; m_medicineTimer.set(2); }
        else if (potencySum == 5) { m_bonusValue = 2; m_medicineTimer.set(2); }
        else if (potencySum == 6) { m_bonusValue = 3; m_medicineTimer.set(2); }
        else if (potencySum == 7) { m_bonusValue = 3; m_medicineTimer.set(3); }
        else if (potencySum == 8) { m_bonusValue = 4; m_medicineTimer.set(3); }
        else if (potencySum == 9) { m_bonusValue = 5; m_medicineTimer.set(3); }
        else if (potencySum == 10) { m_bonusValue = 5; m_medicineTimer.set(4); }
        else if (potencySum == 11) { m_bonusValue = 6; m_medicineTimer.set(4); }
        else if (potencySum == 12) { m_bonusValue = 7; m_medicineTimer.set(4); }
        else if (potencySum == 13) { m_bonusValue = 7; m_medicineTimer.set(5); }
        else if (potencySum == 14) { m_bonusValue = 8; m_medicineTimer.set(5); }
        else { m_bonusValue = 9; m_medicineTimer.set(5); }
    }

    void updateStatus()
    {
        if (m_medicineTimer.hasElapsed())
        {
            m_bonusValue = 0;
        }
        else
        {
            m_medicineTimer.tick();
        }
    }

private:
    int m_bonusValue;
    Timer m_medicineTimer;
};

class HeroMedicineReserve
{
public:
    HeroMedicineReserve(int initialQty = 0, Potency intitialPot1 = Potency::None, Potency intitialPot2 = Potency::None, Potency intitialPot3 = Potency::None) : 
        m_medicineQuantity{initialQty}, ing1{intitialPot1}, ing2{intitialPot2}, ing3{intitialPot3} {}

    bool hasMedicine() const { return m_medicineQuantity > 0; }

    int useUnit()
    {
        if (hasMedicine())
        {
            --m_medicineQuantity;
            return potencyInt(ing1) + potencyInt(ing2) + potencyInt(ing3);
        }
        else return 0;
    }
    void addUnits(int num) { m_medicineQuantity += num; }

private:
    int m_medicineQuantity;
    Potency ing1, ing2, ing3;
};


struct HeroDefenseStatusBar
{
    HeroDefenseStatusBar(const nlohmann::json& fileData, int vitalityValue) :
        m_regenerationStat {fileData.at("regeneration")},
        m_resistanceStat {fileData.at("resistance")},
        m_statusBar {vitalityValue},
        m_medicineStatus {},
        m_medicineReserve {fileData.at("medicine").at("quantity"),
            getPotencyFromString(fileData.at("medicine").at("potency")[0]),
            getPotencyFromString(fileData.at("medicine").at("potency")[1]),
            getPotencyFromString(fileData.at("medicine").at("potency")[2])} {}

    void regenerate()
    {
        m_statusBar.healToMax(calculateHealAmount(m_regenerationStat.statValue(), m_statusBar.max()));
    }

    void checkMedicine()
    {
        if (!m_statusBar.isFull() && !m_medicineStatus.isApplied() && m_medicineReserve.hasMedicine())
        {
            m_medicineStatus.applyMedicine(m_medicineReserve.useUnit());
        }

        m_medicineStatus.updateStatus();

        m_regenerationStat.setMedicineBonus(m_medicineStatus.currentBonus());
    }

    void curseRegen(Potency potency) { m_regenerationStat.curse(potency); }

    void curseResist(Potency potency) { m_resistanceStat.curse(potency); }

    void clearCurses() { m_regenerationStat.clearCurse(); m_resistanceStat.clearCurse(); }

    HeroRegenerationStat m_regenerationStat;
    HeroResistanceStat m_resistanceStat;
    StatusBar m_statusBar;

    HeroMedicineStatus m_medicineStatus;
    HeroMedicineReserve m_medicineReserve;
};


class HeroDefenseStats
{
public:
    HeroDefenseStats(const nlohmann::json& fileData) :
        m_vitality {fileData.at("vitality")},
        m_composure {fileData.at("composure")},
        m_bludgeonHP {fileData.at("defense").at("bludgeon"), m_vitality.statValue()},
        m_poisonHP {fileData.at("defense").at("bludgeon"), m_vitality.statValue()},
        m_shockHP {fileData.at("defense").at("bludgeon"), m_vitality.statValue()},
        m_weaknessHP {fileData.at("defense").at("bludgeon"), m_vitality.statValue()},
        m_bleedHP {fileData.at("defense").at("bludgeon"), m_vitality.statValue()},
        m_corrosiveHP {fileData.at("defense").at("bludgeon"), m_vitality.statValue()},
        m_burnHP {fileData.at("defense").at("bludgeon"), m_vitality.statValue()},
        m_frostbiteHP {fileData.at("defense").at("bludgeon"), m_vitality.statValue()},
        m_psychicHP {fileData.at("defense").at("bludgeon"), m_vitality.statValue()},
        m_vitalHP {m_vitality.statValue()},
        m_vitalCursePotency {Potency::None} {}

    bool rollComposure() const
    {
        return rngCheckPercent(m_composure.statValue());
    }

    void damage(int damageAmount, DamageType type)
    {
        const double resistanceModifier {static_cast<double>(getConstRefToTypedHP(type).m_resistanceStat.statValue()) * 0.01};
        const int damage {damageAmount - static_cast<int>(static_cast<double>(damageAmount) * resistanceModifier)};

        getRefToTypedHP(type).m_statusBar.damage(damage);
        m_vitalHP.damage(static_cast<int>(static_cast<double>(damage) * getVitalCurseModifier()));
    }

    bool isVitalEmpty() const { return m_vitalHP.isEmpty(); }

    bool isEmpty(DamageType type) const
    {
        return getConstRefToTypedHP(type).m_statusBar.isEmpty();
    }

    bool isDamaged(DamageType type) const
    {
        return !getConstRefToTypedHP(type).m_statusBar.isFull();
    }

    void curse(Potency potency, HeroStatType statType, DamageType damageType=DamageType::Psychic)
    {
        if (statType == HeroStatType::Vitality)
        {
            m_vitalCursePotency = potency;
        }
        else if (statType == HeroStatType::Regenerations)
        {
            getRefToTypedHP(damageType).curseRegen(potency);
        }
        else if (statType == HeroStatType::Composure)
        {
            m_composure.curse(potency);
        }
        else if (statType == HeroStatType::Resistances)
        {
            getRefToTypedHP(damageType).curseResist(potency);
        }
    }

    void clearCurses()
    {
        m_vitalCursePotency = Potency::None;
        m_composure.clearCurse();

        m_bludgeonHP.clearCurses();
        m_poisonHP.clearCurses();
        m_shockHP.clearCurses();
        m_weaknessHP.clearCurses();
        m_bleedHP.clearCurses();
        m_corrosiveHP.clearCurses();
        m_burnHP.clearCurses();
        m_frostbiteHP.clearCurses();
        m_psychicHP.clearCurses();
    }

    void checkEachMedicine()
    {
        m_bludgeonHP.checkMedicine();
        m_poisonHP.checkMedicine();
        m_shockHP.checkMedicine();
        m_weaknessHP.checkMedicine();
        m_bleedHP.checkMedicine();
        m_corrosiveHP.checkMedicine();
        m_burnHP.checkMedicine();
        m_frostbiteHP.checkMedicine();
        m_psychicHP.checkMedicine();
    }

    void regenerateEach()
    {
        m_bludgeonHP.regenerate();
        m_poisonHP.regenerate();
        m_shockHP.regenerate();
        m_weaknessHP.regenerate();
        m_bleedHP.regenerate();
        m_corrosiveHP.regenerate();
        m_burnHP.regenerate();
        m_frostbiteHP.regenerate();
        m_psychicHP.regenerate();

        int sumRegenValues {0};
        int numHealingStats {0};
        if (m_bludgeonHP.m_medicineStatus.isApplied()) { sumRegenValues += m_bludgeonHP.m_regenerationStat.statValue(); ++numHealingStats; }
        if (m_poisonHP.m_medicineStatus.isApplied()) { sumRegenValues += m_poisonHP.m_regenerationStat.statValue(); ++numHealingStats; };
        if (m_shockHP.m_medicineStatus.isApplied()) { sumRegenValues += m_shockHP.m_regenerationStat.statValue(); ++numHealingStats; };
        if (m_weaknessHP.m_medicineStatus.isApplied()) { sumRegenValues += m_weaknessHP.m_regenerationStat.statValue(); ++numHealingStats; };
        if (m_bleedHP.m_medicineStatus.isApplied()) { sumRegenValues += m_bleedHP.m_regenerationStat.statValue(); ++numHealingStats; };
        if (m_corrosiveHP.m_medicineStatus.isApplied()) { sumRegenValues += m_corrosiveHP.m_regenerationStat.statValue(); ++numHealingStats; };
        if (m_burnHP.m_medicineStatus.isApplied()) { sumRegenValues += m_burnHP.m_regenerationStat.statValue(); ++numHealingStats; };
        if (m_frostbiteHP.m_medicineStatus.isApplied()) { sumRegenValues += m_frostbiteHP.m_regenerationStat.statValue(); ++numHealingStats; };
        if (m_psychicHP.m_medicineStatus.isApplied()) { sumRegenValues += m_psychicHP.m_regenerationStat.statValue(); ++numHealingStats; };

        if (numHealingStats > 0) m_vitalHP.healToMax(calculateHealAmount(sumRegenValues / numHealingStats, m_vitalHP.max()));
    }

    void statusEffectDamageEach()
    {
        statusEffectDamage(DamageType::Bludgeon);
        statusEffectDamage(DamageType::Poison);
        statusEffectDamage(DamageType::Shock);
        statusEffectDamage(DamageType::Weakness);
        statusEffectDamage(DamageType::Bleed);
        statusEffectDamage(DamageType::Corrosive);
        statusEffectDamage(DamageType::Burn);
        statusEffectDamage(DamageType::Frostbite);
        statusEffectDamage(DamageType::Psychic);
    }

private:
    HeroVitalityStat m_vitality;
    HeroComposureStat m_composure;

    HeroDefenseStatusBar m_bludgeonHP;
    HeroDefenseStatusBar m_poisonHP;
    HeroDefenseStatusBar m_shockHP;
    HeroDefenseStatusBar m_weaknessHP;
    HeroDefenseStatusBar m_bleedHP;
    HeroDefenseStatusBar m_corrosiveHP;
    HeroDefenseStatusBar m_burnHP;
    HeroDefenseStatusBar m_frostbiteHP;
    HeroDefenseStatusBar m_psychicHP;

    StatusBar m_vitalHP;
    Potency m_vitalCursePotency;

    HeroDefenseStatusBar& getRefToTypedHP(DamageType type)
    {
        switch (type)
        {
        case DamageType::Bludgeon: return m_bludgeonHP;
        case DamageType::Poison: return m_poisonHP;
        case DamageType::Shock: return m_shockHP;
        case DamageType::Weakness: return m_weaknessHP;
        case DamageType::Bleed: return m_bleedHP;
        case DamageType::Corrosive: return m_corrosiveHP;
        case DamageType::Burn: return m_burnHP;
        case DamageType::Frostbite: return m_frostbiteHP;
        case DamageType::Psychic: return m_psychicHP;
        default: return m_psychicHP;
        }
    }

    const HeroDefenseStatusBar& getConstRefToTypedHP(DamageType type) const
    {
        switch (type)
        {
        case DamageType::Bludgeon: return m_bludgeonHP;
        case DamageType::Poison: return m_poisonHP;
        case DamageType::Shock: return m_shockHP;
        case DamageType::Weakness: return m_weaknessHP;
        case DamageType::Bleed: return m_bleedHP;
        case DamageType::Corrosive: return m_corrosiveHP;
        case DamageType::Burn: return m_burnHP;
        case DamageType::Frostbite: return m_frostbiteHP;
        case DamageType::Psychic: return m_psychicHP;
        default: return m_psychicHP;
        }
    }

    void statusEffectDamage(DamageType type)
    {
        if (getConstRefToTypedHP(type).m_statusBar.isEmpty()) dealRedirectedDamage(type, getConstRefToTypedHP(type).m_statusBar.current());
    }

    void dealRedirectedDamage(DamageType type, int damage)
    {
        switch(type)
        {
        case DamageType::Bludgeon: { m_poisonHP.m_statusBar.damage(damage); break; }
        case DamageType::Poison: { m_psychicHP.m_statusBar.damage(damage); break; }
        case DamageType::Shock: { m_burnHP.m_statusBar.damage(damage); break; }
        case DamageType::Weakness: { m_frostbiteHP.m_statusBar.damage(damage); break; }
        case DamageType::Bleed: { m_vitalHP.damage(damage); break; }
        case DamageType::Corrosive: { m_bleedHP.m_statusBar.damage(damage); break; }
        case DamageType::Burn: { m_corrosiveHP.m_statusBar.damage(damage); break; }
        case DamageType::Frostbite: { m_corrosiveHP.m_statusBar.damage(damage); break; }
        case DamageType::Psychic: { m_vitalHP.damage(damage); break; }
        }
    }

    double getVitalCurseModifier() const
    {
        switch(m_vitalCursePotency)
        {
            case Potency::Subtle: return 1.1;
            case Potency::Weak: return 1.25;
            case Potency::Average: return 1.5;
            case Potency::Strong: return 2;
            case Potency::Potent: return 3;
            default: return 1;
        }
    }
};


class HeroOffenseStats
{
public:
    HeroOffenseStats(const nlohmann::json& fileData) :
        m_willpower {fileData.at("willpower")},
        m_energy {fileData.at("energy")},
        m_strengthProficiency {fileData.at("offense").at("strength").at("proficiency")},
        m_agilityProficiency {fileData.at("offense").at("agility").at("proficiency")},
        m_intellegenceProficiency {fileData.at("offense").at("intellegence").at("proficiency")},
        m_humanoidProficiency {fileData.at("offense").at("humanoid").at("proficiency")},
        m_naturalProficiency {fileData.at("offense").at("natural").at("proficiency")},
        m_elementalProficiency {fileData.at("offense").at("elemental").at("proficiency")},
        m_monstrousProficiency {fileData.at("offense").at("monstrous").at("proficiency")},
        m_constructedProficiency {fileData.at("offense").at("constructed").at("proficiency")},
        m_astralProficiency {fileData.at("offense").at("astral").at("proficiency")} {}

    int rollProficiencyCheck(int baseStrengthChance, int baseAgilityChance, int baseIntellegenceChance, ThreatType threatType)
    {
        const int strengthDamage {(rollWillpower() && rollStrengthProficiency(baseStrengthChance, threatType)) ? 1 : 0};
        const int agilityDamage {(rollWillpower() && rollAgilityProficiency(baseAgilityChance, threatType)) ? 1 : 0};
        const int intellegenceDamage {(rollWillpower() && rollIntellegenceProficiency(baseIntellegenceChance, threatType)) ? 1 : 0};

        return strengthDamage + agilityDamage + intellegenceDamage;
    }

    void curse(Potency potency, HeroStatType statType, ProficiencyType profType=ProficiencyType::Intellegence)
    {
        if (statType == HeroStatType::Willpower)
        {
            m_willpower.curse(potency);
        }
        else if (statType == HeroStatType::Energy)
        {
            m_energy.curse(potency);
        }
        else if (statType == HeroStatType::Proficiencies)
        {
            if (profType == ProficiencyType::Strength) m_strengthProficiency.curse(potency);
            else if (profType == ProficiencyType::Agility) m_agilityProficiency.curse(potency);
            else if (profType == ProficiencyType::Intellegence) m_intellegenceProficiency.curse(potency);
        }
    }
    void curse(Potency potency, ThreatType threatType)
    {
        if (threatType == ThreatType::Humanoid) m_humanoidProficiency.curse(potency);
        else if (threatType == ThreatType::Natural) m_naturalProficiency.curse(potency);
        else if (threatType == ThreatType::Elemental) m_elementalProficiency.curse(potency);
        else if (threatType == ThreatType::Monstrous) m_monstrousProficiency.curse(potency);
        else if (threatType == ThreatType::Constructed) m_constructedProficiency.curse(potency);
        else if (threatType == ThreatType::Astral) m_astralProficiency.curse(potency);
    }

    void clearCurses()
    {
        m_willpower.clearCurse();
        m_energy.clearCurse();

        m_strengthProficiency.clearCurse();
        m_agilityProficiency.clearCurse();
        m_intellegenceProficiency.clearCurse();
        m_humanoidProficiency.clearCurse();
        m_naturalProficiency.clearCurse();
        m_elementalProficiency.clearCurse();
        m_monstrousProficiency.clearCurse();
        m_constructedProficiency.clearCurse();
        m_astralProficiency.clearCurse();
    }

private:
    HeroWillpowerStat m_willpower;
    HeroEnergyStat m_energy;

    HeroProficiencyStat m_strengthProficiency;
    HeroProficiencyStat m_agilityProficiency;
    HeroProficiencyStat m_intellegenceProficiency;

    HeroProficiencyStat m_humanoidProficiency;
    HeroProficiencyStat m_naturalProficiency;
    HeroProficiencyStat m_elementalProficiency;
    HeroProficiencyStat m_monstrousProficiency;
    HeroProficiencyStat m_constructedProficiency;
    HeroProficiencyStat m_astralProficiency;

    bool rollWillpower() const
    {
        return rngCheckPercent(m_willpower.statValue());
    }

    bool rollStrengthProficiency(int baseClearChance, ThreatType threatType)
    {
        return rngCheckPercent(baseClearChance + getProficiencyValue(ProficiencyType::Strength) + getThreatValue(threatType));
    }

    bool rollAgilityProficiency(int baseClearChance, ThreatType threatType)
    {
        return rngCheckPercent(baseClearChance + getProficiencyValue(ProficiencyType::Agility) + getThreatValue(threatType));
    }

    bool rollIntellegenceProficiency(int baseClearChance, ThreatType threatType)
    {
        return rngCheckPercent(baseClearChance + getProficiencyValue(ProficiencyType::Intellegence) + getThreatValue(threatType));
    }

    int getProficiencyValue(ProficiencyType type)
    {
        switch (type)
        {
        case ProficiencyType::Strength: return m_strengthProficiency.statValue();
        case ProficiencyType::Agility: return m_agilityProficiency.statValue();
        default: return m_intellegenceProficiency.statValue();
        }
    }

    int getThreatValue(ThreatType type)
    {
        switch (type)
        {
        case ThreatType::Humanoid: return m_humanoidProficiency.statValue();
        case ThreatType::Natural: return m_naturalProficiency.statValue();
        case ThreatType::Elemental: return m_elementalProficiency.statValue();
        case ThreatType::Monstrous: return m_monstrousProficiency.statValue();
        case ThreatType::Constructed: return m_constructedProficiency.statValue();
        default: return m_astralProficiency.statValue();
        }
    }
};


class Hero
{
public:
    Hero(const nlohmann::json& fileData) : m_name {fileData.at("name")}, m_defenseStats {fileData.at("stats")}, m_offenseStats{fileData.at("stats")}, m_mortallyWounded{false} {}

    std::string name() const { return m_name; }
    bool isDead() const { return m_mortallyWounded; }

    bool rollPreventDamage() const
    {
        return m_defenseStats.rollComposure();
    }

    void damage(int damageAmount, DamageType type)
    {
        m_defenseStats.damage(damageAmount, type);
    }

    bool hasAction()
    {
        return true;
    }

    void statusConditionDamagePhase()
    {
        m_defenseStats.statusEffectDamageEach();
    }

    int rollProficiencyCheck(int baseStrengthChance, int baseAgilityChance, int baseIntellegenceChance, ThreatType threatType)
    {
        return m_offenseStats.rollProficiencyCheck(baseStrengthChance, baseAgilityChance, baseIntellegenceChance, threatType);
    }

    void takeNewMedicinePhase()
    {
        m_defenseStats.checkEachMedicine();
    }

    void regeneratePhase()
    {
        m_defenseStats.regenerateEach();
    }

    void applyStatusConditionsPhase()
    {
        m_defenseStats.clearCurses();
        m_offenseStats.clearCurses();

        if (m_defenseStats.isEmpty(DamageType::Bludgeon))
        {
            m_defenseStats.curse(Potency::Subtle, HeroStatType::Vitality);
        }
        if (m_defenseStats.isEmpty(DamageType::Poison))
        {
            m_defenseStats.curse(Potency::Average, HeroStatType::Regenerations, DamageType::Poison);
        }
        if (m_defenseStats.isEmpty(DamageType::Shock))
        {
            m_offenseStats.curse(Potency::Subtle, HeroStatType::Energy);
            m_defenseStats.curse(Potency::Weak, HeroStatType::Composure);
        }
        if (m_defenseStats.isEmpty(DamageType::Weakness))
        {
            m_offenseStats.curse(Potency::Subtle, HeroStatType::Energy);
            m_offenseStats.curse(Potency::Weak, HeroStatType::Willpower);
        }
        if (m_defenseStats.isEmpty(DamageType::Bleed))
        {
            m_defenseStats.curse(Potency::Average, HeroStatType::Vitality);
        }
        if (m_defenseStats.isEmpty(DamageType::Corrosive))
        {
            m_defenseStats.curse(Potency::Average, HeroStatType::Regenerations, DamageType::Corrosive);
        }
        if (m_defenseStats.isEmpty(DamageType::Burn))
        {
            m_defenseStats.curse(Potency::Weak, HeroStatType::Composure);
        }
        if (m_defenseStats.isEmpty(DamageType::Frostbite))
        {
            m_offenseStats.curse(Potency::Subtle, HeroStatType::Willpower);
        }
        if (m_defenseStats.isEmpty(DamageType::Psychic))
        {
            m_defenseStats.curse(Potency::Average, HeroStatType::Regenerations, DamageType::Psychic);
        }

        // If Hero is Cursed due to some other mechanism
        // curse stat

        m_mortallyWounded = m_defenseStats.isVitalEmpty();
    }

private:
    std::string m_name;

    HeroDefenseStats m_defenseStats;
    HeroOffenseStats m_offenseStats;

    bool m_mortallyWounded;
};



#endif
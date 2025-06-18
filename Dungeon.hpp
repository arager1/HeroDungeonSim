#ifndef INCLUE_GUARD_DUNGEON_HPP
#define INCLUE_GUARD_DUNGEON_HPP

#include <random>
#include <vector>

#include "Hero.hpp"


class DungeonRoom
{
public: 
    DungeonRoom(const nlohmann::json& fileData) :
        m_chanceDealDamage{fileData.at("damage").at("chance")},
        m_damageMean{fileData.at("damage").at("mean")},
        m_damageStddev{fileData.at("damage").at("stddev")},
        m_damageTypeList{{fileData.at("damage").at("typeChance").at("bludgeon"),
                          fileData.at("damage").at("typeChance").at("poison"),
                          fileData.at("damage").at("typeChance").at("shock"),
                          fileData.at("damage").at("typeChance").at("weakness"),
                          fileData.at("damage").at("typeChance").at("bleed"),
                          fileData.at("damage").at("typeChance").at("corrosive"),
                          fileData.at("damage").at("typeChance").at("burn"),
                          fileData.at("damage").at("typeChance").at("frost"), 
                          fileData.at("damage").at("typeChance").at("psychic")}},
        m_strengthCheckChance{fileData.at("clearing").at("strength")},
        m_agilityCheckChance{fileData.at("clearing").at("agility")},
        m_intellegenceCheckChance{fileData.at("clearing").at("intellegence")},
        m_name{fileData.at("name")},
        m_numTimesCleared{},
        m_numTimesQuit{},
        m_numTimesDied{} {}

    bool attemptRoomClear(Hero& hero)
    {
        bool dungeonAttemptsDamage {rngCheckPercent(m_chanceDealDamage)};
        if (dungeonAttemptsDamage)
        {
            bool heroPreventsDamage {rngCheckPercent(hero.resilliancy())};
            if (!heroPreventsDamage)
            {   
                const int damageAmt {normalDistGenPos(m_damageMean, m_damageStddev)};
                const DamageType damageType {determineDamageType(m_damageTypeList)};
                hero.damage(damageAmt, damageType);
                recordDamage(damageAmt, damageType);
            }
            else
            {
                Logger::log("Hero avoids damage");
            }
        }
        else
        {
            Logger::log("Dungeon does not attempt damage");
        }

        const bool heroSuccess {hero.proficiencyCheck(m_strengthCheckChance, m_agilityCheckChance, m_intellegenceCheckChance)};

        if (heroSuccess)
        {
            Logger::log("Room clear!");
            ++m_numTimesCleared;
            return true;
        }
        else
        {
            Logger::log("All proficiency checks failed. Room not cleared.");
            return false;
        }

        
    }

    std::string name() const {return m_name;}

    // void increaseNumCleared() {++m_numTimesCleared;}
    void increaseNumQuit() {++m_numTimesQuit;}
    void increaseNumDied() {++m_numTimesDied;}

    int getNumCleared() const {return m_numTimesCleared;}
    int getNumQuit() const {return m_numTimesQuit;}
    int getNumDied() const {return m_numTimesDied;}
    int getBludgeonDealt() const {return m_bludgeonDealt;}
    int getPoisonDealt() const {return m_poisonDealt;}
    int getShockDealt() const {return m_shockDealt;}
    int getWeaknessDealt() const {return m_weaknessDealt;}
    int getBleedDealt() const {return m_bleedDealt;}
    int getCorrosiveDealt() const {return m_corrosiveDealt;}
    int getBurnDealt() const {return m_burnDealt;}
    int getFrostbiteDealt() const {return m_frostbiteDealt;}
    int getPsychicDealt() const {return m_psychicDealt;}

private:
    const int m_chanceDealDamage {};
    const int m_damageMean {};
    const int m_damageStddev {};

    const std::vector<int> m_damageTypeList {};

    const int m_strengthCheckChance {};
    const int m_agilityCheckChance {};
    const int m_intellegenceCheckChance {};

    std::string m_name {};

    int m_numTimesCleared {};
    int m_numTimesQuit {};
    int m_numTimesDied {};

    void recordDamage(int amt, DamageType type)
    {
        switch (type)
        {
        case DamageType::Bludgeon: m_bludgeonDealt += amt; break;
        case DamageType::Poison: m_poisonDealt += amt; break;
        case DamageType::Shock: m_shockDealt += amt; break;
        case DamageType::Weakness: m_weaknessDealt += amt; break;
        case DamageType::Bleed: m_bleedDealt += amt; break;
        case DamageType::Corrosive: m_corrosiveDealt += amt; break;
        case DamageType::Burn: m_burnDealt += amt; break;
        case DamageType::Frostbite: m_frostbiteDealt += amt; break;
        case DamageType::Psychic: m_psychicDealt += amt; break;
        }
    }

    int m_bludgeonDealt {};
    int m_poisonDealt {};
    int m_shockDealt {};
    int m_weaknessDealt {};
    int m_bleedDealt {};
    int m_corrosiveDealt {};
    int m_burnDealt {};
    int m_frostbiteDealt {};
    int m_psychicDealt {};
};


class Dungeon
{
public:
    Dungeon(const nlohmann::json& fileData) :
        m_dungeonRoomList {{DungeonRoom(fileData.at("dungeon").at("level0")), 
                            DungeonRoom(fileData.at("dungeon").at("level1")), 
                            DungeonRoom(fileData.at("dungeon").at("level2")),
                            DungeonRoom(fileData.at("dungeon").at("level3")),
                            DungeonRoom(fileData.at("dungeon").at("level4")),
                            DungeonRoom(fileData.at("dungeon").at("level5"))}},
        m_name{fileData.at("dungeon").at("name")} {}

    bool attempt(Hero& hero)
    {
        int levelIndex {0};
        for (DungeonRoom& room : m_dungeonRoomList)
        {
            int attemptCount{1};
            while(hero.isAlive() && hero.willingToAttempt() && !room.attemptRoomClear(hero))
            {
                ++attemptCount;
            }

            if (!hero.isAlive())
            {
                Logger::log("Hero is dead.");
                room.increaseNumDied();
                return false;
            }
            else if (!hero.willingToAttempt())
            {
                Logger::log("Hero is not willing to attempt further.");
                room.increaseNumQuit();
                return false;
            }

            Logger::log("Hero completed Level: " + std::to_string(levelIndex));

            ++levelIndex;
        }

        return true;
    }

    std::string getName() const {return m_name;}

    std::string getStats() const
    {        
        std::stringstream ss{};
        ss << "Name" << "," << "Clear" << "," << "Quit" << "," << "Died" << ",";
        ss << "Bludgeon" << "," << "Poison" << "," << "Shock" << "," << "Weakness" << "," << "Bleed" << ",";
        ss << "Corrossive" << "," << "Burn" << "," << "Frostbite" << "," << "Psychic" << "\n";
        for (const auto& room : m_dungeonRoomList)
        {
            ss << room.name() << "," << room.getNumCleared() << "," << room.getNumQuit() << "," << room.getNumDied() << ",";
            ss << room.getBludgeonDealt() << "," << room.getPoisonDealt() << "," << room.getShockDealt() << ",";
            ss << room.getWeaknessDealt() << "," << room.getBleedDealt() << "," << room.getCorrosiveDealt() << ",";
            ss << room.getBurnDealt() << "," << room.getFrostbiteDealt() << "," << room.getPsychicDealt() << "\n";
        }
        return ss.str();
    }

private:
    std::vector<DungeonRoom> m_dungeonRoomList{};
    std::string m_name{};
};

#endif

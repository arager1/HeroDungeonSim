#ifndef INCLUE_GUARD_DUNGEON_HPP
#define INCLUE_GUARD_DUNGEON_HPP

#include <vector>
#include <map>
#include "Utils.hpp"
#include "Hero.hpp"


constexpr int NUM_SUCCESSES_NEEDED {3};

enum class RoomType
{
    Empty,
    Wellspring,
    Switch,
    Boss
};

enum class RoomLevel
{
    L0, L1, L2, L3, L4, L5
};

class DungeonRoom
{
public:
    DungeonRoom(RoomLevel level=RoomLevel::L0, RoomType type=RoomType::Empty) : m_level{level}, m_type{type}
    {

    }

    void useActionBy(Hero& hero)
    {
        useSubtypeAction(hero);
    }

    bool isHostile() const
    {
        return m_isHostile;
    }

    void regenerate()
    {
        m_isHostile = true;
    }

    void pacify()
    {
        m_isHostile = false;
    }

    RoomLevel level() const
    {
        return m_level;
    }

private:
    bool m_isHostile {true};

    const RoomLevel m_level {};
    const RoomType m_type {};

    virtual void useSubtypeAction(Hero& hero)
    {
        // Do nothing for empty room
    }
};

class DungeonRoomList
{
public:
    DungeonRoomList(RoomLevel level, RoomType type = RoomType::Empty) : m_currentRoomIndex{0}
    {
        addRoom(level, type);
    }

    void addRoom(RoomLevel level, RoomType type = RoomType::Empty)
    {
        m_roomList.push_back(DungeonRoom(level, type));
    }

    RoomLevel currentLevel() const
    {
        return m_roomList[m_currentRoomIndex].level();
    }

    bool isCurrentRoomHostile() const
    {
        return m_roomList[m_currentRoomIndex].isHostile();
    }

    bool allRoomsClear() const
    {
        for (auto& room : m_roomList) if (room.isHostile()) return false;
        return true;
    }

    void pacifyCurrentRoom()
    {
        m_roomList[m_currentRoomIndex].pacify();
    }

    void regenerateRooms(int hostility)
    {
        for (auto& room : m_roomList)
        {
            if (!room.isHostile())
            {
                const int numHostileConnections {rngGenerateInRange(0, 3)}; // TODO
                const int chanceRegenerate {numHostileConnections * hostility};
                if (rngCheckPercent(chanceRegenerate)) room.regenerate();
            }
        }
    }

    bool findNextHostileRoom()
    {
        const int initialIndex {m_currentRoomIndex};
        while (!isCurrentRoomHostile())
        {
            ++m_currentRoomIndex;
            if (m_currentRoomIndex == m_roomList.size()) m_currentRoomIndex = 0;
            if (m_currentRoomIndex == initialIndex) return false;
        }
        return true;
    }

    void useCurrentRoomActionBy(Hero& hero)
    {
        m_roomList[m_currentRoomIndex].useActionBy(hero);
    }

private:
    std::vector<DungeonRoom> m_roomList;
    int m_currentRoomIndex;
};


class DungeonStatBlock
{
public:
    DungeonStatBlock(const nlohmann::json& fileData) :
        m_hostility{fileData.at("hostility")},
        m_volatility{fileData.at("damage").at("chance")},
        m_damageMagnitudeMean{fileData.at("damage").at("mean")},
        m_damageMagnitudeStddev{fileData.at("damage").at("stddev")},
        m_chanceTypeBludgeon{fileData.at("damage").at("typeChance").at("bludgeon")},
        m_chanceTypePoison{fileData.at("damage").at("typeChance").at("poison")},
        m_chanceTypeShock{fileData.at("damage").at("typeChance").at("shock")},
        m_chanceTypeWeakness{fileData.at("damage").at("typeChance").at("weakness")},
        m_chanceTypeBleed{fileData.at("damage").at("typeChance").at("bleed")},
        m_chanceTypeCorrosive{fileData.at("damage").at("typeChance").at("corrosive")},
        m_chanceTypeBurn{fileData.at("damage").at("typeChance").at("burn")},
        m_chanceTypeFrostbite{fileData.at("damage").at("typeChance").at("frost")}, 
        m_chanceTypePsychic{fileData.at("damage").at("typeChance").at("psychic")},
        m_strengthCheckChance{fileData.at("clearing").at("strength")},
        m_agilityCheckChance{fileData.at("clearing").at("agility")},
        m_intellegenceCheckChance{fileData.at("clearing").at("intellegence")},
        m_chanceTypeHumanoid{fileData.at("clearing").at("typeChance").at("humanoid")},
        m_chanceTypeNatural{fileData.at("clearing").at("typeChance").at("natural")},
        m_chanceTypeElemental{fileData.at("clearing").at("typeChance").at("elemental")},
        m_chanceTypeMonstrous{fileData.at("clearing").at("typeChance").at("monstrous")}, 
        m_chanceTypeConstructed{fileData.at("clearing").at("typeChance").at("constructed")},
        m_chanceTypeAstral{fileData.at("clearing").at("typeChance").at("astral")} {}

    bool rollDealDamage() const
    {
        return rngCheckPercent(m_volatility);
    }

    int rollDamageAmount() const
    {
        return normalDistGenPos(m_damageMagnitudeMean, m_damageMagnitudeStddev);
    }

    DamageType rollDamageType() const
    {
        return determineDamageType(
            m_chanceTypeBludgeon,
            m_chanceTypePoison,
            m_chanceTypeShock,
            m_chanceTypeWeakness,
            m_chanceTypeBleed,
            m_chanceTypeCorrosive,
            m_chanceTypeBurn,
            m_chanceTypeFrostbite,
            m_chanceTypePsychic);
    }

    ThreatType rollThreatType() const
    {
        return determineThreatType(
            m_chanceTypeHumanoid,
            m_chanceTypeNatural,
            m_chanceTypeElemental,
            m_chanceTypeMonstrous,
            m_chanceTypeConstructed,
            m_chanceTypeAstral);
    }

    int hostility() const { return m_hostility; }
    int baseStrengthChance() const { return m_strengthCheckChance; }
    int baseAgilityChance() const { return m_agilityCheckChance; }
    int baseIntellegenceChance() const { return m_intellegenceCheckChance; }

private:
    const int m_hostility;
    const int m_volatility;

    const int m_damageMagnitudeMean;
    const int m_damageMagnitudeStddev;

    const int m_chanceTypeBludgeon;
    const int m_chanceTypePoison;
    const int m_chanceTypeShock;
    const int m_chanceTypeWeakness;
    const int m_chanceTypeBleed;
    const int m_chanceTypeCorrosive;
    const int m_chanceTypeBurn;
    const int m_chanceTypeFrostbite;
    const int m_chanceTypePsychic;

    const int m_strengthCheckChance;
    const int m_agilityCheckChance;
    const int m_intellegenceCheckChance;

    const int m_chanceTypeHumanoid;
    const int m_chanceTypeNatural;
    const int m_chanceTypeElemental;
    const int m_chanceTypeMonstrous;
    const int m_chanceTypeConstructed;
    const int m_chanceTypeAstral;
};


class Dungeon
{
public:
    Dungeon(const nlohmann::json& fileData) :
        m_name {fileData.at("dungeon").at("name")},
        m_statBlocklevel0 {fileData.at("dungeon").at("L0")},
        m_statBlocklevel1 {fileData.at("dungeon").at("L1")},
        m_statBlocklevel2 {fileData.at("dungeon").at("L2")},
        m_statBlocklevel3 {fileData.at("dungeon").at("L3")},
        m_statBlocklevel4 {fileData.at("dungeon").at("L4")},
        m_statBlocklevel5 {fileData.at("dungeon").at("L5")},
        m_roomList {RoomLevel::L0}
    {
        m_roomList.addRoom(RoomLevel::L1);
        m_roomList.addRoom(RoomLevel::L2);
        m_roomList.addRoom(RoomLevel::L3);
        m_roomList.addRoom(RoomLevel::L4);
        m_roomList.addRoom(RoomLevel::L5);
    }

    int cycleCount() const { return m_cycleCount; }
    std::string name() const { return m_name; }
    bool isClear() const { return m_roomList.allRoomsClear(); }

    void runCycle(Hero& hero)
    {
        regenerateDungeonPhase();

        // Dungeon Deals Damage Phase
        if (m_roomList.findNextHostileRoom())
        {
            dealDamageTo(hero);
        }

        hero.statusConditionDamagePhase();

        // Hero Action Phase
        if (hero.hasAction())
        {
            if (m_roomList.isCurrentRoomHostile())
            {
                attemptHostileRoomClear(hero);
            }
            else
            {
                useActionOnRoomBy(hero);
            }
        }

        hero.takeNewMedicinePhase();

        hero.regeneratePhase();

        hero.applyStatusConditionsPhase();

        ++m_cycleCount;
    }

private:
    int m_cycleCount {0};
    std::string m_name;

    DungeonStatBlock m_statBlocklevel0;
    DungeonStatBlock m_statBlocklevel1;
    DungeonStatBlock m_statBlocklevel2;
    DungeonStatBlock m_statBlocklevel3;
    DungeonStatBlock m_statBlocklevel4;
    DungeonStatBlock m_statBlocklevel5;

    DungeonRoomList m_roomList;

    int m_heroSuccessCounter {0};

    const DungeonStatBlock& statBlock(RoomLevel level)
    {
        switch (level)
        {
        case RoomLevel::L1: return m_statBlocklevel1;
        case RoomLevel::L2: return m_statBlocklevel2;
        case RoomLevel::L3: return m_statBlocklevel3;
        case RoomLevel::L4: return m_statBlocklevel4;
        case RoomLevel::L5: return m_statBlocklevel5;
        default: return m_statBlocklevel0;
        }
    }

    const DungeonStatBlock& currStatBlock()
    {
        return statBlock(m_roomList.currentLevel());
    }

    int getHostility(RoomLevel level)
    {
        return statBlock(level).hostility();
    }

    void regenerateDungeonPhase()
    {
        m_roomList.regenerateRooms(getHostility(m_roomList.currentLevel()));
    }

    void dealDamageTo(Hero& hero)
    {
        const bool dungeonAttemptsDamage {currStatBlock().rollDealDamage()};
        const bool heroPreventsDamage {hero.rollPreventDamage()};

        if (dungeonAttemptsDamage && !heroPreventsDamage)
        {
            const int damageAmt {currStatBlock().rollDamageAmount()};
            const DamageType damageType {currStatBlock().rollDamageType()};
            hero.damage(damageAmt, damageType);
        }
    }

    void attemptHostileRoomClear(Hero& hero)
    {
        m_heroSuccessCounter += hero.rollProficiencyCheck(
            currStatBlock().baseStrengthChance(),
            currStatBlock().baseAgilityChance(),
            currStatBlock().baseIntellegenceChance(),
            currStatBlock().rollThreatType());

        if (m_heroSuccessCounter >= NUM_SUCCESSES_NEEDED)
        {
            m_roomList.pacifyCurrentRoom();
            m_heroSuccessCounter = 0;
        }
    }

    void useActionOnRoomBy(Hero& hero)
    {
        m_roomList.useCurrentRoomActionBy(hero);
    }
};

#endif

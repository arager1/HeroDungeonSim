#ifndef INCLUE_GUARD_LOGGER_HPP
#define INCLUE_GUARD_LOGGER_HPP

#include <random>
#include <iostream>
#include <sstream>

#include "json.hpp"


enum class DamageType
{
    Bludgeon,
    Poison,
    Shock,
    Weakness,
    Bleed,
    Corrosive,
    Burn,
    Frostbite,
    Psychic,
};

enum class ProficiencyType
{
    Strength,
    Agility,
    Intellegence
};

enum class ThreatType
{
    Humanoid,
    Natural,
    Elemental,
    Monstrous,
    Constructed,
    Astral
};

enum class Potency
{
    None,
    Subtle,
    Weak,
    Average,
    Strong,
    Potent
};

enum class StatLevel
{
    MinusTen, MinusNine, MinusEight, MinusSeven, MinusSix, MinusFive, MinusFour, MinusThree, MinusTwo, MinusOne,
    Base,
    PlusOne, PlusTwo, PlusThree, PlusFour, PlusFive, PlusSix, PlusSeven, PlusEight, PlusNine, PlusTen
};

enum class HeroStatType
{
    Vitality,
    Regenerations,
    Composure,
    Resistances,
    Willpower,
    Proficiencies,
    Energy
};

bool operator <(Potency lhs, Potency rhs)
{
    return static_cast<int>(lhs) < static_cast<int>(rhs);
}

StatLevel convertStatLevelFromInt(int levelSum, int min=-10, int max=10)
{
    constexpr int absMin {-10}, absMax {10}; 

    const int newMin {(min < absMin) ? absMin : min}, newMax {(max > absMax) ? absMax : max};

    const int newLevelSum {(levelSum < newMin) ? newMin : (levelSum > newMax) ? newMax : levelSum};

    switch (newLevelSum)
    {
    case absMin: return StatLevel::MinusTen;
    case -9: return StatLevel::MinusNine;
    case -8: return StatLevel::MinusEight;
    case -7: return StatLevel::MinusSeven;
    case -6: return StatLevel::MinusSix;
    case -5: return StatLevel::MinusFive;
    case -4: return StatLevel::MinusFour;
    case -3: return StatLevel::MinusThree;
    case -2: return StatLevel::MinusTwo;
    case -1: return StatLevel::MinusOne;
    case 0: return StatLevel::Base;
    case 1: return StatLevel::PlusOne;
    case 2: return StatLevel::PlusTwo;
    case 3: return StatLevel::PlusThree;
    case 4: return StatLevel::PlusFour;
    case 5: return StatLevel::PlusFive;
    case 6: return StatLevel::PlusSix;
    case 7: return StatLevel::PlusSeven;
    case 8: return StatLevel::PlusEight;
    case 9: return StatLevel::PlusNine;
    case absMax: return StatLevel::PlusTen;
    default: return StatLevel::Base;
    }
}

int potencyInt(Potency potency)
{
    switch (potency)
    {
    case Potency::Subtle: return 1;
    case Potency::Weak: return 2;
    case Potency::Average: return 3;
    case Potency::Strong: return 4;
    case Potency::Potent: return 5;
    default: return 0;
    }
}

std::string getDamageTypeName(DamageType type)
{
    switch (type)
    {
    case DamageType::Bludgeon: return "Bludgeon";
    case DamageType::Poison: return "Poison";
    case DamageType::Shock: return "Shock";
    case DamageType::Weakness: return "Weakness";
    case DamageType::Bleed: return "Bleed";
    case DamageType::Corrosive: return "Corrosive";
    case DamageType::Burn: return "Burn";
    case DamageType::Frostbite: return "Frostbite";
    case DamageType::Psychic: return "Psychic";
    default: return "Unknown";
    }
}

std::string getProficiencyTypeName(ProficiencyType type)
{
    switch (type)
    {
    case ProficiencyType::Strength: return "Strength";
    case ProficiencyType::Agility: return "Agility";
    case ProficiencyType::Intellegence: return "Intellegence";
    default: return "Unknown";
    }
}

Potency getPotencyFromString(std::string str)
{
    if (str == "Subtle") return Potency::Subtle;
    else if (str == "Weak") return Potency::Weak;
    else if (str == "Average") return Potency::Average;
    else if (str == "Strong") return Potency::Strong;
    else if (str == "Potent") return Potency::Potent;
    else return Potency::None;
}

std::random_device device;
std::mt19937 rngGen(device());

int rngGenerateInRange(int min, int rangeSize)
{
    std::uniform_int_distribution<> dist(min, min+rangeSize-1);
    return dist(rngGen);
}

int rngGenerateBetweenOneAndHundred()
{
    return rngGenerateInRange(1, 100);
}

// Generate a number [1, 100] and
//    return true IF that number is less than or equal to that
//    return false IF that number is greater than that
bool rngCheckPercent(int percentThreshold)
{
    return rngGenerateBetweenOneAndHundred() <= percentThreshold;
}

int normalDistGenPos(int mean, int stddev)
{
    std::normal_distribution<> dist(mean, stddev);
    const int sample {static_cast<int>(dist(rngGen))};
    return (sample > 0) ? sample : 0;
}


DamageType determineDamageType(const std::vector<int>& damageTypeList)
{
    std::discrete_distribution<int> dist{damageTypeList.begin(), damageTypeList.end()};
    return static_cast<DamageType>(dist(rngGen));
}

class DamageTypeDist
{
public:
    DamageTypeDist(int chanceTypeBludgeon, int chanceTypePoison, int chanceTypeShock, int chanceTypeWeakness, int chanceTypeBleed, int chanceTypeCorrosive, int chanceTypeBurn, int chanceTypeFrostbite, int chanceTypePsychic) :
        m_damageTypeList {{chanceTypeBludgeon, chanceTypePoison, chanceTypeShock, chanceTypeWeakness, chanceTypeBleed, chanceTypeCorrosive, chanceTypeBurn, chanceTypeFrostbite, chanceTypePsychic}} {}

    DamageType generateType() const
    {
        std::discrete_distribution<int> dist{m_damageTypeList.begin(), m_damageTypeList.end()};
        switch (dist(rngGen))
        {
        case 0: return DamageType::Bludgeon;
        case 1: return DamageType::Poison;
        case 2: return DamageType::Shock;
        case 3: return DamageType::Weakness;
        case 4: return DamageType::Bleed;
        case 5: return DamageType::Corrosive;
        case 6: return DamageType::Burn;
        case 7: return DamageType::Frostbite;
        default: return DamageType::Psychic;
        }
    }

private:
    const std::vector<int> m_damageTypeList;
};

DamageType determineDamageType(int chanceTypeBludgeon, int chanceTypePoison, int chanceTypeShock, int chanceTypeWeakness, int chanceTypeBleed, int chanceTypeCorrosive, int chanceTypeBurn, int chanceTypeFrostbite, int chanceTypePsychic)
{
    const DamageTypeDist damageTypeDist {chanceTypeBludgeon, chanceTypePoison, chanceTypeShock, chanceTypeWeakness, chanceTypeBleed, chanceTypeCorrosive, chanceTypeBurn, chanceTypeFrostbite, chanceTypePsychic};
    return damageTypeDist.generateType();
}

class ThreatTypeDist
{
public:
    ThreatTypeDist(int chanceTypeHumanoid, int chanceTypeNatural, int chanceTypeElemental, int chanceTypeMonstrous, int chanceTypeConstructed, int chanceTypeAstral) :
        m_threatTypeList {{chanceTypeHumanoid, chanceTypeNatural, chanceTypeElemental, chanceTypeMonstrous, chanceTypeConstructed, chanceTypeAstral}} {}

    ThreatType generateType() const
    {
        std::discrete_distribution<int> dist{m_threatTypeList.begin(), m_threatTypeList.end()};
        switch (dist(rngGen))
        {
        case 0: return ThreatType::Humanoid;
        case 1: return ThreatType::Natural;
        case 2: return ThreatType::Elemental;
        case 3: return ThreatType::Monstrous;
        case 4: return ThreatType::Constructed;
        default: return ThreatType::Astral;
        }
    }

private:
    const std::vector<int> m_threatTypeList;
};

ThreatType determineThreatType(int chanceTypeHumanoid, int chanceTypeNatural, int chanceTypeElemental, int chanceTypeMonstrous, int chanceTypeConstructed, int chanceTypeAstral)
{
    const ThreatTypeDist damageTypeList {chanceTypeHumanoid, chanceTypeNatural, chanceTypeElemental, chanceTypeMonstrous, chanceTypeConstructed, chanceTypeAstral};
    return damageTypeList.generateType();
}

int calculateHealAmount(int regenStatValue, int maxHP)
{
    const double regenModifier {static_cast<double>(regenStatValue) * 0.01};
    return static_cast<int>(static_cast<double>(maxHP) * regenModifier);
}

int getPercent(int num, int den)
{
    return static_cast<int>(100.0 * (static_cast<float>(num) / static_cast<float>(den)));
}

std::string getPercentStr(int num, int den)
{
    return std::to_string(getPercent(num, den)) + "\%";
}

class Timer
{
public:
    void set(int threshold) { m_threshold = threshold; }
    void tick() { ++m_counter; }
    bool hasElapsed() const { return m_counter >= m_threshold; }

private:
    int m_counter {0};
    int m_threshold {0};
};

class Logger
{
public:
    static void log(std::string msg)
    {
        std::cout << msg << std::endl;
    }

    // static void printLog()
    // {
    //     std::cout << m_log.str() << std::endl;
    // }

    static void logBegin(std::string dungeonName, std::string heroName, int attemptNum=0)
    {
        log("\n\nAttempt number " + std::to_string(attemptNum) + " for " + dungeonName);
        log("Attempted by: " + heroName);
    }

    static void logDamage(int damage, DamageType type, int heroOverallHP, int heroOverallConstitution, int heroTypedHP, int heroTypedConstitution)
    {
        log("Hero takes " + std::to_string(damage) + " points of " + getDamageTypeName(type) + " damage");
        log("Hero has " + std::to_string(heroOverallHP) + "/" + std::to_string(heroOverallConstitution) + " overall HP and " + 
            std::to_string(heroTypedHP) + "/" + std::to_string(heroTypedConstitution) + " " + getDamageTypeName(type) + " HP");
    }

    static void logHP(std::string label, int cur, int max)
    {   
        std::stringstream ss;
        ss << label << ": [";

        const int percent {static_cast<int>(100.0 * (static_cast<float>(cur) / static_cast<float>(max)))};
        for (int i{0}; i<percent; ++i) ss << "#";
        for (int i{0}; i<(100-percent); ++i) ss << " ";

        ss << "]";

        log(ss.str());
    }

    static void logHeroStatus()
    {

    }

private:
    // static std::stringstream m_log;
};


#endif

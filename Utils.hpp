#ifndef INCLUE_GUARD_LOGGER_HPP
#define INCLUE_GUARD_LOGGER_HPP

#include <iostream>
#include <sstream>


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
    Psychic
};

enum class ProficiencyType
{
    Strength,
    Agility,
    Intellegence
};

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

std::random_device device;
std::mt19937 rngGen(device());

int rngGenerateBetweenOneAndHundred()
{
    std::uniform_int_distribution<> dist(1, 100);
    return dist(rngGen);
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

int getPercent(int num, int den)
{
    return static_cast<int>(100.0 * (static_cast<float>(num) / static_cast<float>(den)));
}

std::string getPercentStr(int num, int den)
{
    return std::to_string(getPercent(num, den)) + "\%";
}

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

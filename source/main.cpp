
#include <fstream>
#include <filesystem>

#include "Dungeon.hpp"

class HeroFactory
{
public:
    HeroFactory(std::ifstream infile) : m_heroData{nlohmann::json::parse(infile)} {}

    Hero makeHero() const {return Hero{m_heroData};}

    std::string heroName() const { return makeHero().name(); }

private:
    nlohmann::json m_heroData{};
};

class DungeonFactory
{
public:
    DungeonFactory(std::ifstream infile) : m_dungeonData{nlohmann::json::parse(infile)} {}

    Dungeon makeDungeon() const {return Dungeon{m_dungeonData};}

    std::string dungeonName() const { return makeDungeon().name(); }

private:
    nlohmann::json m_dungeonData{};
};


void runDungeonSequence(const DungeonFactory& dungeonFactory, const HeroFactory& heroFactory, std::stringstream& ss, int numHeroes = 1)
{
    int cycleCountSum {0};
    int numDead {0};
    int cycleCountDead {0};
    int numClear {0};
    int cycleCountClear {0};
    for (int i {0}; i<numHeroes; ++i)
    {
        Dungeon dungeon{dungeonFactory.makeDungeon()};
        Hero hero{heroFactory.makeHero()};

        while (!dungeon.isClear() && !hero.isDead())
        {
            dungeon.runCycle(hero);
        }

        cycleCountSum += dungeon.cycleCount();
        if (hero.isDead())
        {
            ++numDead;
            cycleCountDead += dungeon.cycleCount();
        }
        if (dungeon.isClear())
        {
            ++numClear;
            cycleCountClear += dungeon.cycleCount();
        }
    }

    const int overallTimeAverage {numHeroes > 0 ? cycleCountSum / numHeroes : 0};
    const int clearingTimeAverage {numClear > 0 ? cycleCountClear / numClear : 0};
    const int deathTimeAverage {numDead > 0 ? cycleCountDead / numDead : 0};

    ss << dungeonFactory.dungeonName() << "," << heroFactory.heroName() << "\n";
    ss << "Attempted,AverageTime,Cleared,AverageClearingTime,Died,AverageDeathTime" << "\n";
    ss << numHeroes << "," << overallTimeAverage << "," << numClear << "," << clearingTimeAverage << "," << numDead << "," << deathTimeAverage << "\n";
}


int main(int argc, char* argv[])
{
    const std::filesystem::path outputLogFilename {std::filesystem::current_path() / "GameLog.csv"};
    std::stringstream output{};

    DungeonFactory bigDungeonFactory {std::ifstream{std::filesystem::current_path() / "Dungeons" / "TheBigDungeon.json"}};
    HeroFactory weakHeroFactory {std::ifstream{std::filesystem::current_path() / "Heroes" / "weakHero.json"}};
    HeroFactory midHeroFactory {std::ifstream{std::filesystem::current_path() / "Heroes" / "midHero.json"}};
    HeroFactory strongHeroFactory {std::ifstream{std::filesystem::current_path() / "Heroes" / "strongHero.json"}};

    const int numHeroes {1000};


    runDungeonSequence(bigDungeonFactory, weakHeroFactory, output, numHeroes);
    runDungeonSequence(bigDungeonFactory, midHeroFactory, output, numHeroes);
    runDungeonSequence(bigDungeonFactory, strongHeroFactory, output, numHeroes);

    // Dungeon dungeon{dungeonFactory.makeDungeon()};
    // int numConsecutiveHeros {0};
    // while (!dungeon.isClear())
    // {
    //     Hero hero{heroFactory.makeHero()};
    //     ++numConsecutiveHeros;
    //     while (!hero.isDead())
    //     {
    //         dungeon.runCycle(hero);
    //         if (dungeon.isClear()) break;
    //     }
    // }

    // output << "Consecutive Heroes Needed for 1 Dungeon" << "\n" << numConsecutiveHeros << "\n";

    std::ofstream outfile{outputLogFilename};
    outfile << output.str();
    return 0;
}


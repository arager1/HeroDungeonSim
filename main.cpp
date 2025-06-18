
#include <fstream>

#include "Dungeon.hpp"


class HeroFactory
{
public:
    HeroFactory(std::ifstream infile) : m_heroData{nlohmann::json::parse(infile).at("hero")} {}

    Hero makeHero() const {return Hero{m_heroData};}

private:
    nlohmann::json m_heroData{};
};


int main(int argc, char* argv[])
{
    const std::string dungeonFilename {"../Dungeons/testDungeon.json"};
    const std::string heroFilename {"../Heroes/testHero.json"};

    Dungeon dungeon(nlohmann::json::parse(std::ifstream{dungeonFilename}));
    HeroFactory heroFactory {std::ifstream{heroFilename}};

    const int numAttempts{(argc > 1) ? std::stoi(argv[1]) : 500};
    for (int i{0}; i<numAttempts; ++i)
    {
        Hero hero{heroFactory.makeHero()};

        Logger::logBegin(dungeon.getName(), hero.name(), (i + 1));

        if (dungeon.attempt(hero))
        {
            Logger::log("Hero completed the dungeon!");
        }
        else
        {
            Logger::log("Hero did not complete the dungeon.");
        }
    }

    Logger::log(dungeon.getStats());
    std::ofstream outfile{"../GameLog.csv"};
    outfile << dungeon.getStats();

    return 0;    
}


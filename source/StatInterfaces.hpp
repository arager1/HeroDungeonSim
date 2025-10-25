#ifndef INCLUE_GUARD_STATINTERFACES_HPP
#define INCLUE_GUARD_STATINTERFACES_HPP

#include "Stats.hpp"


bool rollDungeonRegenerates(const DungeonAlarmStat& dungeonAlarm, const DungeonRageStat& dungeonRage, int numHostileConnections)
{
    const int chanceRegenerate {dungeonAlarm.chanceRehostileValue() + dungeonRage.bonusRehostileValue()};

    for (int i=0; i<numHostileConnections; ++i) if (rngCheckPercent(chanceRegenerate)) return true;

    return false;
}

bool rollDungeonDealsDamage(const DungeonAlarmStat& dungeonAlarm, const HeroComposureStat& heroComposure)
{
    const int chanceDamage {dungeonAlarm.chanceDamageValue() - heroComposure.statValue()};

    return rngCheckPercent(chanceDamage);
}

bool rollDamageMagnitude(const DungeonRageStat& dungeonRage, const DungeonChaosStat& dungeonChaos, const HeroResistanceStat& heroResistance)
{
    const int dungeonDamage {normalDistGenPos(dungeonRage.damageMeanValue(), dungeonChaos.damageStddevValue(dungeonRage.damageMeanValue()))};

    const double heroResistanceModifier {static_cast<double>(heroResistance.statValue()) * 0.01};

    return dungeonDamage - static_cast<int>(static_cast<double>(dungeonDamage) * heroResistanceModifier);
}


DamageType rollDamageType(const DungeonChaosStat& dungeonChaos, const std::vector<DamageTypeDist>& damageTypeMatrix)
{
    return damageTypeMatrix[dungeonChaos.damageTypeIndexValue()].generateType();
}

ThreatType rollThreatType(const DungeonChaosStat& dungeonChaos, const std::vector<ThreatTypeDist>& threatTypeMatrix)
{
    return threatTypeMatrix[dungeonChaos.threatTypeIndexValue()].generateType();
}

int rollHeroSkillChecks(const DungeonObstacleStat& dungeonStrengthObstacle,
                        const DungeonObstacleStat& dungeonAgilityObstacle,
                        const DungeonObstacleStat& dungeonIntellegenceObstacle,
                        const HeroWillpowerStat& heroWillpower,
                        const HeroProficiencyStat& heroStrengthProficiency,
                        const HeroProficiencyStat& heroAgilityProficiency,
                        const HeroProficiencyStat& heroIntellegenceProficiency,
                        const HeroProficiencyStat& heroThreatProficiency)
{
    const int strengthChance {dungeonStrengthObstacle.chanceClearValue() + heroStrengthProficiency.statValue() + heroThreatProficiency.statValue()};
    const int agilityChance {dungeonAgilityObstacle.chanceClearValue() + heroAgilityProficiency.statValue() + heroThreatProficiency.statValue()};
    const int intellegenceChance {dungeonIntellegenceObstacle.chanceClearValue() + heroIntellegenceProficiency.statValue() + heroThreatProficiency.statValue()};

    const int strengthDamage {(rngCheckPercent(heroWillpower.statValue()) && rngCheckPercent(strengthChance)) ? 1 : 0};
    const int agilityDamage {(rngCheckPercent(heroWillpower.statValue()) && rngCheckPercent(agilityChance)) ? 1 : 0};
    const int intellegenceDamage {(rngCheckPercent(heroWillpower.statValue()) && rngCheckPercent(intellegenceChance)) ? 1 : 0};

    return strengthDamage + agilityDamage + intellegenceDamage;
}

#endif

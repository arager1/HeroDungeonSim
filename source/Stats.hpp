#ifndef INCLUE_GUARD_STATS_HPP
#define INCLUE_GUARD_STATS_HPP

#include "Utils.hpp"

class GenericStat
{
public:
    GenericStat(int initLevel) : m_baseStatLevel{initLevel} {}

    StatLevel statLevel() const { return convertStatLevelFromInt(statLevelInt(), min(), max()); }

    void levelUp(int amt=1) { m_baseStatLevel += (amt < 0) ? 0 : amt; }

    void modify(int amt) { m_modifierSum += amt; }

private:
    int m_baseStatLevel;

    int m_modifierSum {0};

    virtual int min() const = 0;
    virtual int max() const = 0;

    int statLevelInt() const
    {
        return m_baseStatLevel + m_modifierSum;
    }
};

class HeroStat : public GenericStat
{
public:
    HeroStat(int initLevel) : GenericStat(initLevel) {}

    Potency cursePotency() const { return m_curseLevel; }

    void curse(Potency potency)
    {
        if (m_curseLevel < potency)
        {
            m_curseLevel = potency;
            modify(curseLevelValue());
        }
    }

    void clearCurse()
    {
        modify (-1 * curseLevelValue());
        m_curseLevel = Potency::None;
    }

private:
    Potency m_curseLevel {Potency::None};

    virtual int curseLevelValue() const = 0;
};


class DungeonStat : public GenericStat
{
public:
    DungeonStat(int initLevel) : GenericStat(initLevel) {}
};

// ------------------------------ Hero Stats -----------------------------------------------


class HeroRegenerationStat : public HeroStat
{
public:
    HeroRegenerationStat(int initalExpLevel) : HeroStat(initalExpLevel) {}

    int statValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return -5;
        case StatLevel::MinusTwo: return -1;
        case StatLevel::MinusOne: return 0;
        case StatLevel::Base: return 1;
        case StatLevel::PlusOne: return 2;
        case StatLevel::PlusTwo: return 5;
        case StatLevel::PlusThree: return 10;
        case StatLevel::PlusFour: return 15;
        case StatLevel::PlusFive: return 20;
        case StatLevel::PlusSix: return 30;
        case StatLevel::PlusSeven: return 40;
        case StatLevel::PlusEight: return 60;
        case StatLevel::PlusNine: return 80;
        default: return 100;
        }
    }

    void setMedicineBonus(int level)
    {
        const int bonusLevelDelta {level - m_medicineBonus};
        modify(bonusLevelDelta);
        m_medicineBonus = level;
    }

private:
    int min() const override { return -3; }
    int max() const override { return 10; }
    int curseLevelValue() const override
    {
        switch(cursePotency())
        {
        case Potency::Subtle: return -1;
        case Potency::Weak: return -4;
        case Potency::Average: return -7;
        case Potency::Strong: return -10;
        case Potency::Potent: return -13;
        default: return 0;
        }
    }

    int m_medicineBonus {0};
};

class HeroVitalityStat : public HeroStat
{
public:
    HeroVitalityStat(int initalExpLevel) : HeroStat(initalExpLevel) {}

    int statValue() const
    {
        switch(statLevel())
        {
        case StatLevel::Base: return 10;
        case StatLevel::PlusOne: return 15;
        case StatLevel::PlusTwo: return 25;
        case StatLevel::PlusThree: return 35;
        case StatLevel::PlusFour: return 50;
        case StatLevel::PlusFive: return 75;
        case StatLevel::PlusSix: return 125;
        case StatLevel::PlusSeven: return 175;
        case StatLevel::PlusEight: return 275;
        case StatLevel::PlusNine: return 400;
        default: return 600;
        }
    }

private:
    int min() const override { return 0; }
    int max() const override { return 10; }
    int curseLevelValue() const override { return 0; }
};

class HeroComposureStat : public HeroStat
{
public:
    HeroComposureStat(int initalExpLevel) : HeroStat(initalExpLevel) {}

    int statValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return -30;
        case StatLevel::MinusTwo: return -20;
        case StatLevel::MinusOne: return -10;
        case StatLevel::Base: return 0;
        case StatLevel::PlusOne: return 10;
        case StatLevel::PlusTwo: return 20;
        case StatLevel::PlusThree: return 30;
        case StatLevel::PlusFour: return 40;
        case StatLevel::PlusFive: return 50;
        case StatLevel::PlusSix: return 60;
        case StatLevel::PlusSeven: return 70;
        case StatLevel::PlusEight: return 80;
        case StatLevel::PlusNine: return 90;
        default: return 100;
        }
    }

private:
    int min() const override { return -3; }
    int max() const override { return 10; }
    int curseLevelValue() const override
    {
        switch(cursePotency())
        {
        case Potency::Subtle: return -1;
        case Potency::Weak: return -3;
        case Potency::Average: return -5;
        case Potency::Strong: return -7;
        case Potency::Potent: return -9;
        default: return 0;
        }
    }
};

class HeroResistanceStat : public HeroStat
{
public:
    HeroResistanceStat(int initalExpLevel) : HeroStat(initalExpLevel) {}

    int statValue() const
    {
        switch(statLevel())
        {
        case StatLevel::Base: return 0;
        case StatLevel::PlusOne: return 5;
        case StatLevel::PlusTwo: return 10;
        case StatLevel::PlusThree: return 15;
        case StatLevel::PlusFour: return 20;
        case StatLevel::PlusFive: return 25;
        case StatLevel::PlusSix: return 30;
        case StatLevel::PlusSeven: return 40;
        case StatLevel::PlusEight: return 50;
        case StatLevel::PlusNine: return 60;
        default: return 70;
        }
    }

private:
    int min() const override { return 0; }
    int max() const override { return 10; }
    int curseLevelValue() const override
    {
        switch(cursePotency())
        {
        case Potency::Subtle: return -1;
        case Potency::Weak: return -2;
        case Potency::Average: return -3;
        case Potency::Strong: return -4;
        case Potency::Potent: return -5;
        default: return 0;
        }
    }
};

class HeroWillpowerStat : public HeroStat
{
public:
    HeroWillpowerStat(int initalExpLevel) : HeroStat(initalExpLevel) {}

    int statValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return 0;
        case StatLevel::MinusTwo: return 5;
        case StatLevel::MinusOne: return 15;
        case StatLevel::Base: return 25;
        case StatLevel::PlusOne: return 35;
        case StatLevel::PlusTwo: return 45;
        case StatLevel::PlusThree: return 55;
        case StatLevel::PlusFour: return 65;
        case StatLevel::PlusFive: return 75;
        case StatLevel::PlusSix: return 80;
        case StatLevel::PlusSeven: return 85;
        case StatLevel::PlusEight: return 90;
        case StatLevel::PlusNine: return 95;
        default: return 100;
        }
    }

private:
    int min() const override { return -3; }
    int max() const override { return 10; }
    int curseLevelValue() const override
    {
        switch(cursePotency())
        {
        case Potency::Subtle: return -1;
        case Potency::Weak: return -3;
        case Potency::Average: return -5;
        case Potency::Strong: return -7;
        case Potency::Potent: return -9;
        default: return 0;
        }
    }
};

class HeroProficiencyStat : public HeroStat
{
public:
    HeroProficiencyStat(int initalExpLevel) : HeroStat(initalExpLevel) {}

    int statValue() const
    {
        switch(statLevel())
        {
        case StatLevel::Base: return -10;
        case StatLevel::PlusOne: return -5;
        case StatLevel::PlusTwo: return 0;
        case StatLevel::PlusThree: return 5;
        case StatLevel::PlusFour: return 10;
        case StatLevel::PlusFive: return 15;
        case StatLevel::PlusSix: return 20;
        case StatLevel::PlusSeven: return 25;
        case StatLevel::PlusEight: return 30;
        case StatLevel::PlusNine: return 40;
        default: return 50;
        }
    }

private:
    int min() const override { return 0; }
    int max() const override { return 10; }
    int curseLevelValue() const override
    {
        switch(cursePotency())
        {
        case Potency::Subtle: return -1;
        case Potency::Weak: return -2;
        case Potency::Average: return -3;
        case Potency::Strong: return -4;
        case Potency::Potent: return -5;
        default: return 0;
        }
    }
};

class HeroEnergyStat : public HeroStat
{
public:
    HeroEnergyStat(int initalExpLevel) : HeroStat(initalExpLevel) {}

    int statValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return -1;
        case StatLevel::MinusTwo: return 0;
        case StatLevel::MinusOne: return 1;
        case StatLevel::Base: return 2;
        case StatLevel::PlusOne: return 3;
        case StatLevel::PlusTwo: return 4;
        default: return 5;
        }
    }

private:
    int min() const override { return -3; }
    int max() const override { return 3; }
    int curseLevelValue() const override
    {
        switch(cursePotency())
        {
        case Potency::Subtle: return -1;
        case Potency::Weak: return -2;
        case Potency::Average: return -3;
        case Potency::Strong: return -4;
        case Potency::Potent: return -5;
        default: return 0;
        }
    }
};


// ------------------------------ Dungeon Stats --------------------------------------------



class DungeonRageStat : public DungeonStat
{
public:
    DungeonRageStat(int initalLevel) : DungeonStat(initalLevel) {}

    int damageMeanValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return 0;
        case StatLevel::MinusTwo: return 1;
        case StatLevel::MinusOne: return 2;
        case StatLevel::Base: return 4;
        case StatLevel::PlusOne: return 6;
        case StatLevel::PlusTwo: return 8;
        case StatLevel::PlusThree: return 10;
        case StatLevel::PlusFour: return 15;
        case StatLevel::PlusFive: return 20;
        case StatLevel::PlusSix: return 25;
        case StatLevel::PlusSeven: return 30;
        case StatLevel::PlusEight: return 35;
        case StatLevel::PlusNine: return 40;
        default: return 50;
        }
    }

    int bonusRehostileValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return -5;
        case StatLevel::MinusTwo: return -2;
        case StatLevel::MinusOne: return -1;
        case StatLevel::Base: return 0;
        case StatLevel::PlusOne: return 1;
        case StatLevel::PlusTwo: return 2;
        case StatLevel::PlusThree: return 3;
        case StatLevel::PlusFour: return 4;
        case StatLevel::PlusFive: return 5;
        case StatLevel::PlusSix: return 7;
        case StatLevel::PlusSeven: return 9;
        case StatLevel::PlusEight: return 11;
        case StatLevel::PlusNine: return 13;
        default: return 15;
        }
    }

    int bonusChanceClearValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return 3;
        case StatLevel::MinusTwo: return 2;
        case StatLevel::MinusOne: return 1;
        case StatLevel::Base: return 0;
        case StatLevel::PlusOne: return -1;
        case StatLevel::PlusTwo: return -2;
        case StatLevel::PlusThree: return -3;
        case StatLevel::PlusFour: return -4;
        case StatLevel::PlusFive: return -5;
        case StatLevel::PlusSix: return -6;
        case StatLevel::PlusSeven: return -7;
        case StatLevel::PlusEight: return -8;
        case StatLevel::PlusNine: return -9;
        default: return -10;
        }
    }

private:
    int min() const override { return -3; }
    int max() const override { return 10; }
};


class DungeonAlarmStat : public DungeonStat
{
public:
    DungeonAlarmStat(int initalLevel) : DungeonStat(initalLevel) {}

    int chanceDamageValue() const
    {
        switch(statLevel())
        {
        case StatLevel::Base: return 0;
        case StatLevel::PlusOne: return 5;
        case StatLevel::PlusTwo: return 15;
        case StatLevel::PlusThree: return 15;
        case StatLevel::PlusFour: return 25;
        case StatLevel::PlusFive: return 25;
        case StatLevel::PlusSix: return 45;
        case StatLevel::PlusSeven: return 45;
        case StatLevel::PlusEight: return 75;
        case StatLevel::PlusNine: return 75;
        default: return 100;
        }
    }

    int chanceRehostileValue() const
    {
        switch(statLevel())
        {
        case StatLevel::Base: return 0;
        case StatLevel::PlusOne: return 5;
        case StatLevel::PlusTwo: return 5;
        case StatLevel::PlusThree: return 10;
        case StatLevel::PlusFour: return 10;
        case StatLevel::PlusFive: return 15;
        case StatLevel::PlusSix: return 15;
        case StatLevel::PlusSeven: return 20;
        case StatLevel::PlusEight: return 20;
        case StatLevel::PlusNine: return 25;
        default: return 25;
        }
    }

    int bonusRageLevelValue() const
    {
        switch(statLevel())
        {
        case StatLevel::Base: return 0;
        case StatLevel::PlusOne: return 0;
        case StatLevel::PlusTwo: return 0;
        case StatLevel::PlusThree: return 0;
        case StatLevel::PlusFour: return 0;
        case StatLevel::PlusFive: return 1;
        case StatLevel::PlusSix: return 1;
        case StatLevel::PlusSeven: return 1;
        case StatLevel::PlusEight: return 2;
        case StatLevel::PlusNine: return 2;
        default: return 2;
        }
    }

private:
    int min() const override { return 0; }
    int max() const override { return 10; }
};


class DungeonChaosStat : public DungeonStat
{
public:
    DungeonChaosStat(int initalLevel) : DungeonStat(initalLevel) {}

    int damageStddevValue(int meanValue) const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return 0;
        case StatLevel::MinusTwo: return meanValue / 6;
        case StatLevel::MinusOne: return meanValue / 5;
        case StatLevel::Base: return meanValue / 4;
        case StatLevel::PlusOne: return meanValue / 3;
        case StatLevel::PlusTwo: return meanValue / 2;
        default: return meanValue;
        }
    }

    int damageTypeIndexValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return 0;
        case StatLevel::MinusTwo: return 1;
        case StatLevel::MinusOne: return 2;
        case StatLevel::Base: return 3;
        case StatLevel::PlusOne: return 4;
        case StatLevel::PlusTwo: return 5;
        default: return 6;
        }
    }

    int threatTypeIndexValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return 0;
        case StatLevel::MinusTwo: return 1;
        case StatLevel::MinusOne: return 2;
        case StatLevel::Base: return 3;
        case StatLevel::PlusOne: return 4;
        case StatLevel::PlusTwo: return 5;
        default: return 6;
        }
    }

private:
    int min() const override { return -3; }
    int max() const override { return 3; }
};


class DungeonObstacleStat : public DungeonStat
{
public:
    DungeonObstacleStat(int initalLevel) : DungeonStat(initalLevel) {}

    int chanceClearValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return 100;
        case StatLevel::MinusTwo: return 90;
        case StatLevel::MinusOne: return 80;
        case StatLevel::Base: return 70;
        case StatLevel::PlusOne: return 60;
        case StatLevel::PlusTwo: return 50;
        case StatLevel::PlusThree: return 40;
        case StatLevel::PlusFour: return 30;
        case StatLevel::PlusFive: return 25;
        case StatLevel::PlusSix: return 20;
        case StatLevel::PlusSeven: return 15;
        case StatLevel::PlusEight: return 10;
        case StatLevel::PlusNine: return 5;
        default: return 0;
        }
    }

    int lootAddedPerSkillValue() const
    {
        switch(statLevel())
        {
        case StatLevel::MinusThree: return 0;
        case StatLevel::MinusTwo: return 1;
        case StatLevel::MinusOne: return 5;
        case StatLevel::Base: return 10;
        case StatLevel::PlusOne: return 20;
        case StatLevel::PlusTwo: return 30;
        case StatLevel::PlusThree: return 50;
        case StatLevel::PlusFour: return 70;
        case StatLevel::PlusFive: return 100;
        case StatLevel::PlusSix: return 200;
        case StatLevel::PlusSeven: return 300;
        case StatLevel::PlusEight: return 500;
        case StatLevel::PlusNine: return 700;
        default: return 1000;
        }
    }

private:
    int min() const override { return -3; }
    int max() const override { return 10; }
};


class DungeonScaleStat : public DungeonStat
{
public:
    DungeonScaleStat(int initalLevel) : DungeonStat(initalLevel) {}

    int roomCountValue() const
    {
        switch(statLevel())
        {
        case StatLevel::Base: return 1;
        case StatLevel::PlusOne: return 2;
        case StatLevel::PlusTwo: return 3;
        case StatLevel::PlusThree: return 4;
        case StatLevel::PlusFour: return 5;
        case StatLevel::PlusFive: return 6;
        case StatLevel::PlusSix: return 8;
        case StatLevel::PlusSeven: return 10;
        case StatLevel::PlusEight: return 15;
        case StatLevel::PlusNine: return 20;
        default: return 25;
        }
    }

    int bonusLootPercentValue() const
    {
        switch(statLevel())
        {
        case StatLevel::Base: return 1;
        case StatLevel::PlusOne: return 2;
        case StatLevel::PlusTwo: return 3;
        case StatLevel::PlusThree: return 4;
        case StatLevel::PlusFour: return 5;
        case StatLevel::PlusFive: return 6;
        case StatLevel::PlusSix: return 8;
        case StatLevel::PlusSeven: return 10;
        case StatLevel::PlusEight: return 15;
        case StatLevel::PlusNine: return 20;
        default: return 25;
        }
    }

private:
    int min() const override { return 0; }
    int max() const override { return 10; }
};


#endif

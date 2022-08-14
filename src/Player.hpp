#pragma once

#include "Actor.hpp"

class Player : public Actor
{
  public:
    Player(Point const& point) : Actor(Actor::Create(Actor::Type::Player, point))
    {
    }

    bool Gain(int xp)
    {
        if (xp == 0 || m_levelUpBase == 0)
        {
            return false;
        }

        m_currentXp += xp;

        if (m_currentXp >= XpToNextLevel())
        {
            ++m_level;
            if (m_level % 2 == 0)
            {
                ++m_fighter.m_power;
            }
            else
            {
                ++m_fighter.m_defense;
            }
            return true;
        }

        return false;
    }

    int XpToNextLevel() const
    {
        return XpToLevel(m_level);
    }

    int XpToPreviousLevel() const
    {
        return XpToLevel(m_level - 1);
    }

    int XpToLevel(int level) const
    {
        if (level == 0)
        {
            return 0;
        }

        return m_levelUpBase + level * m_levelUpFactor;
    }

    int m_level = 1;
    int m_currentXp = 0;
    int m_levelUpBase = 200;
    int m_levelUpFactor = 150;
};

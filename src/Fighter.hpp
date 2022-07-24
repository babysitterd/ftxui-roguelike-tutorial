#pragma once

#include <cmath>

class Fighter
{
  public:
    Fighter(int hpFull, int defense, int power)
        : m_hpFull(hpFull), m_hpCurrent(hpFull), m_defense(defense), m_power(power)
    {
    }

    void SetHitpoints(int hp)
    {
        if (hp < 0)
        {
            m_hpCurrent = 0;
        }
        else if (hp > m_hpFull)
        {
            m_hpCurrent = m_hpFull;
        }
        else
        {
            m_hpCurrent = hp;
        }
    }

    int m_hpFull;
    int m_hpCurrent;
    int m_defense;
    int m_power;
};

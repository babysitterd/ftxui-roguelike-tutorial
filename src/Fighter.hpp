#pragma once

#include <cmath>

class Fighter
{
  public:
    Fighter(int hpFull, int defense, int power)
        : m_hpFull(hpFull), m_hpCurrent(hpFull), m_defense(defense), m_power(power)
    {
    }

    int Heal(int amount)
    {
        auto const current = m_hpCurrent;
        SetHitpoints(current + amount);
        return m_hpCurrent - current;
    }

    void TakeDamage(int amount)
    {
        SetHitpoints(m_hpCurrent - amount);
    }

    int m_hpFull;
    int m_hpCurrent;
    int m_defense;
    int m_power;

  private:
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
};

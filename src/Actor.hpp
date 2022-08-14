#pragma once

#include "Entity.hpp"
#include "Fighter.hpp"
#include "Point.hpp"

class Actor : public Entity
{
  public:
    enum class Type
    {
        Player,
        Orc,
        Troll
    };

    Actor(Type type, Point const& point, char codepoint, ftxui::Color const& color,
          Fighter const& fighter, int xpGiven);

    static Actor Create(Type type, Point const& point);

    std::string Name() const;

    void Die();
    bool IsDead() const;

    Type m_type;
    Point m_point;
    Fighter m_fighter;
    int m_xpGiven;
};

// allows look up std::set by Point
struct Compare
{
    using is_transparent = void;

    bool operator()(Actor const& lhs, Actor const& rhs) const
    {
        return lhs.m_point < rhs.m_point;
    }

    bool operator()(Point const& lhs, Actor const& rhs) const
    {
        return lhs < rhs.m_point;
    }

    bool operator()(Actor const& lhs, Point const& rhs) const
    {
        return lhs.m_point < rhs;
    }
};

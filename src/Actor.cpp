#include "Actor.hpp"

#include <stdexcept>

Actor::Actor(Type type, Point const& point, char codepoint, ftxui::Color const& color,
             Fighter const& fighter, int xpGiven)
    : Entity(codepoint, color), m_type(type), m_point(point), m_fighter(fighter), m_xpGiven(xpGiven)
{
}

Actor Actor::Create(Actor::Type type, Point const& point)
{
    switch (type)
    {
    case Type::Player:
        return Actor{type, point, '@', ftxui::Color::White, Fighter{30, 2, 5}, 0};
    case Type::Orc:
        return Actor{type, point, 'o', ftxui::Color::LightGreen, Fighter{10, 0, 3}, 35};
    case Type::Troll:
        return Actor{type, point, 'T', ftxui::Color::GreenYellow, Fighter{16, 1, 4}, 100};
    default:
        throw std::invalid_argument{"Unknown Actor type"};
    }
}

std::string Actor::Name() const
{
    switch (m_type)
    {
    case Type::Player:
        return "PLAYER";
    case Type::Orc:
        return "ORC";
    case Type::Troll:
        return "TROLL";
    default:
        throw std::invalid_argument{"Unknown Actor type"};
    }
}

void Actor::Die()
{
    m_codepoint = '%';
    m_color = ftxui::Color::Red;
}

bool Actor::IsDead() const
{
    return m_codepoint == '%';
}

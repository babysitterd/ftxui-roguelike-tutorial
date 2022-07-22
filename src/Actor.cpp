#include "Actor.hpp"

#include <stdexcept>

Actor::Actor(Type type, Point const& point, char codepoint, ftxui::Color const& color)
    : Entity(codepoint, color), m_type(type), m_point(point)
{
}

Actor Actor::Create(Actor::Type type, Point const& point)
{
    switch (type)
    {
    case Type::Player:
        return Actor{type, point, '@', ftxui::Color::White};
    case Type::Orc:
        return Actor{type, point, 'o', ftxui::Color::LightGreen};
    case Type::Troll:
        return Actor{type, point, 'T', ftxui::Color::GreenYellow};
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

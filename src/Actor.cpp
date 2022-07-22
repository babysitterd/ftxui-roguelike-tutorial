#include "Actor.hpp"

#include <stdexcept>

Actor::Actor(Point const& point, char codepoint, ftxui::Color const& color)
    : Entity(codepoint, color), m_point(point)
{
}

Actor Actor::Create(Actor::Type type, Point const& point)
{
    switch (type)
    {
    case Type::Player:
        return Actor{point, '@', ftxui::Color::White};
    case Type::Orc:
        return Actor{point, 'o', ftxui::Color::LightGreen};
    case Type::Troll:
        return Actor{point, 'T', ftxui::Color::GreenYellow};
    default:
        throw std::invalid_argument{"Unknown Actor type"};
    }
}

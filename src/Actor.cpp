#include "Actor.hpp"

Actor::Actor(Point const& point, char codepoint, ftxui::Color const& color)
    : Entity(codepoint, color), m_point(point)
{
}

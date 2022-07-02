#include "Actor.hpp"

Actor::Actor(Point const& point, char codepoint, ftxui::Color const& color)
    : m_point(point), m_codepoint(codepoint), m_color(color)
{
}

ftxui::Element Actor::Render() const
{
    return ftxui::text(std::string{m_codepoint}) | ftxui::color(m_color);
}

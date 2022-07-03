#include "Entity.hpp"

Entity::Entity(char codepoint, ftxui::Color const& color) : m_codepoint(codepoint), m_color(color)
{
}

ftxui::Element Entity::Render() const
{
    return ftxui::text(std::string{m_codepoint}) | ftxui::color(m_color);
}

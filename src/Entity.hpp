#pragma once

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

class Entity
{
  public:
    Entity(char codepoint, ftxui::Color const& color);
    virtual ~Entity() = default;

    char Codepoint() const;

    ftxui::Element Render() const;

  protected:
    char m_codepoint;
    ftxui::Color m_color;
};

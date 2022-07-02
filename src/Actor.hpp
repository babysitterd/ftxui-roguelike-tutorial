#pragma once

#include "Point.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/color.hpp>

class Actor
{
  public:
    Actor(Point const& point, char codepoint, ftxui::Color const& color);
    ftxui::Element render() const;

    Point m_point;
    char m_codepoint;
    ftxui::Color m_color;
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

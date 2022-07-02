#pragma once

#include "Entity.hpp"
#include "Point.hpp"

class Actor : public Entity
{
  public:
    Actor(Point const& point, char codepoint, ftxui::Color const& color);

    Point m_point;
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

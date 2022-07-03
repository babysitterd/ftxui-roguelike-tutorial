#pragma once

#include "Point.hpp"

struct Room
{
    Room(int x, int y, int width, int height)
        : m_northWest{x, y}, m_southEast{x + width, y + height}
    {
    }

    Point Center() const
    {
        return {(m_northWest.x + m_southEast.x) / 2, (m_northWest.y + m_southEast.y) / 2};
    }

    bool Intersects(Room const& other) const
    {
        return m_northWest.x <= other.m_southEast.x && m_southEast.x >= other.m_northWest.x &&
               m_northWest.y <= other.m_southEast.y && m_southEast.y >= other.m_northWest.y;
    }

    Point m_northWest;
    Point m_southEast;
};

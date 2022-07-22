#pragma once

#include <compare>

struct Point
{
    int x = 0;
    int y = 0;

    Point() = default;
    Point(int _x, int _y) : x(_x), y(_y)
    {
    }

    Point(Point const&) = default;
    Point& operator=(Point const&) = default;

    Point(Point&&) = default;
    Point& operator=(Point&&) = default;

    // Whoaa SPACESHIP is here!
    auto operator<=>(Point const&) const = default;
};

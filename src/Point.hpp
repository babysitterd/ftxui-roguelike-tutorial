#pragma once

#include <compare>

struct Point
{
    int x = 0;
    int y = 0;
    // Whoaa SPACESHIP is here!
    auto operator<=>(Point const&) const = default;
};

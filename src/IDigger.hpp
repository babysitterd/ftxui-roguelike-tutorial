#pragma once

#include "Room.hpp"

struct IDigger
{
    virtual ~IDigger() = default;

    virtual void CarveRoom(Room const& room) = 0;
    virtual void DigHorizontalTunnel(int x1, int x2, int y) = 0;
    virtual void DigVerticalTunnel(int y1, int y2, int x) = 0;
};

#pragma once

#include "Point.hpp"

struct Room;

class IMapMutator
{
  public:
    virtual ~IMapMutator() = default;

    virtual void CarveRoom(Room const& room) = 0;
    virtual void DigHorizontalTunnel(int x1, int x2, int y) = 0;
    virtual void DigVerticalTunnel(int y1, int y2, int x) = 0;
    virtual void DigDownstairs(Point const& point) = 0;
};

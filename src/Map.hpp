#pragma once

#include "Entity.hpp"
#include "Point.hpp"
#include "Room.hpp"
#include "Tile.hpp"

#include <ftxui/dom/elements.hpp>

#include <vector>

class Map
{
  public:
    Map(int width, int height);

    void CarveRoom(Room const& room);
    void DigHorizontalTunnel(int x1, int x2, int y);
    void DigVerticalTunnel(int y1, int y2, int x);

    Tile const& At(Point const& point) const;
    bool IsOutOfBounds(Point const& point) const;

    ftxui::Element Render(Point const& point) const;

    int m_width;
    int m_height;
    std::vector<Tile> m_tiles;
    std::vector<Room> m_rooms;
};

#pragma once

#include "Entity.hpp"
#include "Point.hpp"
#include "Room.hpp"
#include "Tile.hpp"

#include <ftxui/dom/elements.hpp>

#include <iosfwd>
#include <string>
#include <vector>

class Map
{
  public:
    template <class T>
    Map(int width, int height, T&& generator)
        : m_width(width), m_height(height), m_tiles(m_width * m_height)
    {
        m_rooms = generator.Generate(*this);
    }

    explicit Map(std::vector<std::string> const& map);

    void CarveRoom(Room const& room);
    void DigHorizontalTunnel(int x1, int x2, int y);
    void DigVerticalTunnel(int y1, int y2, int x);

    Tile const& At(Point const& point) const;
    bool IsOutOfBounds(Point const& point) const;

    ftxui::Element Render(Point const& point) const;

    friend std::ostream& operator<<(std::ostream& os, Map const& map);

    int m_width;
    int m_height;
    std::vector<Tile> m_tiles;
    std::vector<Room> m_rooms;
};

std::ostream& operator<<(std::ostream& os, Map const& map);

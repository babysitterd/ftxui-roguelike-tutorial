#include "Map.hpp"

#include <algorithm>

#include <ftxui/screen/color.hpp>

Map::Map(int width, int height) : m_width(width), m_height(height), m_tiles(m_width * m_height)
{
    constexpr int roomMaxSize = 10;
    constexpr int roomMinSize = 6;
    constexpr int maxRooms = 30;

    MapGenerator generator(width, height);
    m_rooms = generator.Generate(roomMaxSize, roomMinSize, maxRooms, *this);
}

Tile const& Map::At(Point const& point) const
{
    return m_tiles[point.x + point.y * m_width];
}

bool Map::IsOutOfBounds(Point const& point) const
{
    return point.x < 0 || point.x >= m_width || point.y < 0 || point.y >= m_height;
}

void Map::CarveRoom(Room const& room)
{
    for (int i = room.m_northWest.x + 1; i < room.m_southEast.x; ++i)
    {
        for (int j = room.m_northWest.y; j < room.m_southEast.y; ++j)
        {
            m_tiles[i + j * m_width] = Tile::Create(Tile::Type::Floor);
        }
    }
}

void Map::DigHorizontalTunnel(int x1, int x2, int y)
{
    auto const [xfrom, xto] = std::minmax(x1, x2);
    for (int i = xfrom; i <= xto; ++i)
    {
        m_tiles[i + y * m_width] = Tile::Create(Tile::Type::Floor);
    }
}

void Map::DigVerticalTunnel(int y1, int y2, int x)
{
    auto const [yfrom, yto] = std::minmax(y1, y2);
    for (int i = yfrom; i <= yto; ++i)
    {
        m_tiles[x + i * m_width] = Tile::Create(Tile::Type::Floor);
    }
}

ftxui::Element Map::Render(Point const& point) const
{
    return At(point).Render();
}

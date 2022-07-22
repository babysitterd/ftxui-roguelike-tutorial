#include "Map.hpp"

#include "NaiveMapGenerator.hpp"

#include <algorithm>
#include <iostream>

#include <ftxui/screen/color.hpp>

namespace
{

constexpr int RoomMaxSize = 10;
constexpr int RoomMinSize = 6;
constexpr int MaxRooms = 30;

} // namespace

Map::Map(int width, int height, MapGeneratorBase& generator)
    : m_width(width), m_height(height), m_tiles(m_width * m_height)
{
    m_rooms = generator.Generate(*this);
}

Map::Map(int width, int height, RNG& rng)
    : m_width(width), m_height(height), m_tiles(m_width * m_height)
{
    NaiveMapGenerator generator(width, height, RoomMaxSize, RoomMinSize, MaxRooms, rng);
    m_rooms = generator.Generate(*this);
}

Map::Map(std::vector<std::string> const& map)
    : m_width(static_cast<int>(map.back().size())), m_height(static_cast<int>(map.size())),
      m_tiles(m_width * m_height)
{
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            m_tiles[x + y * m_width] = Tile::Create(map[y][x]);
        }
    }
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

std::ostream& Map::WriteToStream(std::ostream& os) const
{
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            os << m_tiles[x + y * m_width].Codepoint();
        }
        os << '\n';
    }
    os << '\n';
    return os;
}

std::ostream& operator<<(std::ostream& os, Map const& map)
{
    return map.WriteToStream(os);
}

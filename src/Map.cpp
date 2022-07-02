#include "Map.hpp"

#include <ftxui/screen/color.hpp>

#include <stdexcept>

Tile::Tile() : Tile(Tile::Create(Tile::Type::Floor))
{
}

Tile::Tile(Tile::Type type, bool canWalk) : m_type(type), m_canWalk(canWalk)
{
}

Tile Tile::Create(enum class Tile::Type type)
{
    switch (type)
    {
    case Type::Floor:
        return Tile{type, true};
    case Type::Wall:
    case Type::Void:
        return Tile{type, false};
    default:
        throw std::invalid_argument{"Unknown Tile type"};
    }
}

bool Tile::CanWalk() const
{
    return m_canWalk;
}

Tile::Type Tile::GetType() const
{
    return m_type;
}

Map::Map(int width, int height) : m_width(width), m_height(height), m_tiles(m_width * m_height)
{
    setWall({30, 22});
    setWall({50, 22});
}

Tile const& Map::At(Point const& point) const
{
    return m_tiles[point.x + point.y * m_width];
}

bool Map::IsOutOfBounds(Point const& point) const
{
    return point.x < 0 || point.x >= m_width || point.y < 0 || point.y >= m_height;
}

void Map::setWall(Point const& point)
{
    m_tiles[point.x + point.y * m_width] = Tile::Create(Tile::Type::Wall);
}

ftxui::Element Map::Render(Point const& point) const
{
    static const ftxui::Color darkWall = ftxui::Color::CadetBlue;
    static const ftxui::Color darkGround(50, 50, 150);

    if (At(point).GetType() == Tile::Type::Wall)
    {
        return ftxui::text(std::string{'#'}) | ftxui::color(darkWall);
    }
    else
    {
        return ftxui::text(std::string{'.'}) | ftxui::color(darkGround);
    }
}

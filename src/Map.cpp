#include "Map.hpp"

#include <ftxui/screen/color.hpp>

Map::Map(int width, int height) :
    m_width(width), m_height(height), m_tiles(m_width * m_height)
{
    setWall({30, 22});
    setWall({50, 22});
}

bool Map::isWall(Point const& point) const
{
    auto const index = point.x + point.y * m_width;
    if (index < 0 || index >= m_tiles.size())
    {
        return false;
    }

    return !m_tiles[index].canWalk;
}

void Map::setWall(Point const& point) {
   m_tiles[point.x + point.y * m_width].canWalk = false;
}

ftxui::Element Map::render(Point const& point) const
{
    static const ftxui::Color darkWall = ftxui::Color::CadetBlue;
    static const ftxui::Color darkGround(50,50,150);

    if (isWall(point)) {
        return ftxui::text(std::string{'#'}) | ftxui::color(darkWall);
    } else {
        return ftxui::text(std::string{'.'}) | ftxui::color(darkGround);
    }
}

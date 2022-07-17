#include "FovMap.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>

FovMap::FovMap(std::vector<std::string> const& map, int radius)
    : Map(map), m_light(m_tiles.size(), false), m_radius(radius)
{
}

FovMap::FovMap(int width, int height, int fovRadius)
    : Map(width, height), m_light(m_tiles.size(), false), m_radius(fovRadius)
{
}

void FovMap::LineOfSight(Point const& viewpoint)
{
    std::fill(m_light.begin(), m_light.end(), false);
    auto const pi = std::acos(-1.);
    std::set<Point> circle;
    for (double angle = 0; angle < 2 * pi; angle += 0.05)
    {
        double x = viewpoint.x + std::cos(angle) * m_radius;
        double y = viewpoint.y + std::sin(angle) * m_radius;
        circle.emplace(static_cast<int>(x), static_cast<int>(y));
    }

    for (auto const& point : circle)
    {
        Bresenham(viewpoint, point);
    }
}

//  https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void FovMap::Bresenham(Point from, Point to)
{
    int const dx = std::abs(to.x - from.x);
    int const sx = from.x < to.x ? 1 : -1;
    int const dy = (-1) * std::abs(to.y - from.y);
    int const sy = from.y < to.y ? 1 : -1;
    int error = dx + dy;

    bool isBlocked = false;
    for (;;)
    {
        if (IsOutOfBounds(from))
        {
            return;
        }

        if (isBlocked)
        {
            m_light[from.x + from.y * m_width] = false;
        }
        else
        {
            m_light[from.x + from.y * m_width] = true;
            m_tiles[from.x + from.y * m_width].Explore();
            if (!m_tiles[from.x + from.y * m_width].CanWalk())
            {
                isBlocked = true;
            }
        }

        if (from == to)
        {
            return;
        }

        if (2 * error >= dy)
        {
            if (from.x == to.x)
            {
                return;
            }
            error += dy;
            from.x += sx;
        }

        if (2 * error <= dx)
        {
            if (from.y == to.y)
            {
                return;
            }
            error += dx;
            from.y += sy;
        }
    }
}

ftxui::Element FovMap::Render(Point const& point) const
{
    if (At(point).IsExplored())
    {
        return m_light[point.x + point.y * m_width] ? At(point).RenderLight() : At(point).Render();
    }

    return Tile::Create(Tile::Type::Void).Render();
}

std::ostream& FovMap::WriteToStream(std::ostream& os) const
{
    for (int y = 0; y < m_height; ++y)
    {
        for (int x = 0; x < m_width; ++x)
        {
            os << (m_light[x + y * m_width] ? m_tiles[x + y * m_width].Codepoint() : '%');
        }
        os << '\n';
    }
    os << '\n';
    return os;
}

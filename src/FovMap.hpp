#pragma once

#include "Map.hpp"
#include "Point.hpp"
#include "RNG.hpp"

#include <iosfwd>
#include <string>
#include <vector>

class FovMap : public Map
{
  public:
    FovMap(std::vector<std::string> const& map, int radius);
    FovMap(int width, int height, int fovRadius, RNG& rng);

    void LineOfSight(Point const& viewpoint);

    void Bresenham(Point from, Point to);

    ftxui::Element Render(Point const& point) const;

    std::ostream& WriteToStream(std::ostream& os) const override;

    std::vector<bool> m_light;
    int m_radius;
};
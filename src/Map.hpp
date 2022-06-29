#pragma once

#include "Point.hpp"

#include <ftxui/dom/elements.hpp>

#include <vector>

struct Tile {
   bool canWalk; // can we walk through this tile?
   Tile() : canWalk(true) {}
};

class Map {
public :
   Map(int width, int height);

   bool isWall(Point const& point) const;
   void setWall(Point const& point);

   ftxui::Element render(Point const& point) const;

   int m_width;
   int m_height;
   std::vector<Tile> m_tiles;
};

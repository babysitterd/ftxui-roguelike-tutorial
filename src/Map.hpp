#pragma once

#include "Point.hpp"

#include <ftxui/dom/elements.hpp>

#include <vector>

class Tile
{
  public:
    enum class Type
    {
        Floor,
        Wall,
        Void
    };

    Tile();
    static Tile Create(enum class Type type);

    Type GetType() const;
    bool CanWalk() const;

  private:
    explicit Tile(Type type, bool canWalk);

    Type m_type;
    bool m_canWalk;
};

class Map
{
  public:
    Map(int width, int height);

    bool isWall(Point const& point) const;
    void setWall(Point const& point);

    ftxui::Element render(Point const& point) const;

    int m_width;
    int m_height;
    std::vector<Tile> m_tiles;
};

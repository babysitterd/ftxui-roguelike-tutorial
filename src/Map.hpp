#pragma once

#include "Entity.hpp"
#include "Point.hpp"

#include <ftxui/dom/elements.hpp>

#include <vector>

class Tile : public Entity
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
    explicit Tile(Type type, bool canWalk, char codepoint, ftxui::Color const& color);

    Type m_type;
    bool m_canWalk;
};

class Map
{
  public:
    Map(int width, int height);

    void setWall(Point const& point);
    Tile const& At(Point const& point) const;
    bool IsOutOfBounds(Point const& point) const;

    ftxui::Element Render(Point const& point) const;

    int m_width;
    int m_height;
    std::vector<Tile> m_tiles;
};

#pragma once

#include "Entity.hpp"

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
    static Tile Create(Type type);

    Type GetType() const;
    bool CanWalk() const;

  private:
    explicit Tile(Type type, bool canWalk, char codepoint, ftxui::Color const& color);

    Type m_type;
    bool m_canWalk;
};

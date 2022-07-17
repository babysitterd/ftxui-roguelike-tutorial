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
    static Tile Create(Type type, bool light = false);
    static Tile Create(char codepoint);

    ftxui::Element RenderLight() const;

    void Explore();

    Type GetType() const;
    bool CanWalk() const;
    bool IsExplored() const;

  private:
    explicit Tile(Type type, bool canWalk, char codepoint, ftxui::Color const& color);

    Type m_type;
    bool m_canWalk;
    bool m_isExplored;
};

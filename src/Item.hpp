#pragma once

#include "Actor.hpp"
#include "Entity.hpp"
#include "MessageLog.hpp"
#include "Point.hpp"

#include <functional>

class World;

using Effect = std::function<void(World&, Point const&)>;

class Item : public Entity
{
  public:
    enum class Type
    {
        HealthPotion,
        LightningScroll,
        FireballScroll
    };

    Item(Type type, Point const& point, char codepoint, ftxui::Color const& color, Effect effect,
         bool isTargetSpell = false, int radius = 0);

    static Item Create(Type type, Point const& point);

    std::string Name() const;

    Type m_type;
    Point m_point;
    Effect m_effect;
    bool m_isTargetSpell;
    int m_radius;
};

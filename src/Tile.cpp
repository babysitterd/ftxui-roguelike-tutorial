#include "Tile.hpp"

#include <stdexcept>

namespace Color
{

namespace Dark
{

const ftxui::Color Floor{50, 50, 150};
const ftxui::Color Wall{0, 0, 100};

} // namespace Dark

namespace Light
{

const ftxui::Color Floor{200, 180, 50};
const ftxui::Color Wall{130, 110, 50};

} // namespace Light

} // namespace Color

Tile::Tile() : Tile(Tile::Create(Tile::Type::Wall))
{
}

Tile::Tile(Tile::Type type, bool canWalk, char codepoint, ftxui::Color const& color)
    : Entity(codepoint, color), m_type(type), m_canWalk(canWalk), m_isExplored(false)
{
}

Tile Tile::Create(Tile::Type type, bool light)
{
    switch (type)
    {
    case Type::Floor:
        return Tile{type, true, '.', light ? Color::Light::Floor : Color::Dark::Floor};
    case Type::Wall:
        return Tile{type, false, '#', light ? Color::Light::Wall : Color::Dark::Wall};
    case Type::Void:
        return Tile{type, false, ' ', ftxui::Color::Black};
    default:
        throw std::invalid_argument{"Unknown Tile type"};
    }
}

Tile Tile::Create(char codepoint)
{
    switch (codepoint)
    {
    case '.':
        return Tile{Type::Floor, true, codepoint, Color::Light::Floor};
    case '#':
        return Tile{Type::Wall, false, codepoint, Color::Light::Wall};
    case ' ':
        return Tile{Type::Void, false, codepoint, ftxui::Color::Black};
    default:
        throw std::invalid_argument{"Unknown Tile type"};
    }
}

ftxui::Element Tile::RenderLight() const
{
    auto light = Tile::Create(m_type, true);
    return light.Render();
}

void Tile::Explore()
{
    m_isExplored = true;
}

bool Tile::CanWalk() const
{
    return m_canWalk;
}

Tile::Type Tile::GetType() const
{
    return m_type;
}

bool Tile::IsExplored() const
{
    return m_isExplored;
}

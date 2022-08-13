#include "Tile.hpp"

#include <stdexcept>

namespace Color
{

namespace Dark
{

const ftxui::Color Floor{ftxui::Color::Cyan2};
const ftxui::Color Wall{0, 128, 128};
const ftxui::Color Downstairs{50, 50, 150};

} // namespace Dark

namespace Light
{

const ftxui::Color Floor{ftxui::Color::Yellow2};
const ftxui::Color Wall{ftxui::Color::Yellow1};
const ftxui::Color Downstairs{ftxui::Color::Yellow3};

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
    case Type::Downstairs:
        return Tile{type, true, '>', light ? Color::Light::Downstairs : Color::Dark::Downstairs};
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
    case '>':
        return Tile{Type::Downstairs, true, codepoint, Color::Light::Downstairs};
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

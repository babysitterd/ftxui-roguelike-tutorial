#include "Tile.hpp"

#include <stdexcept>

Tile::Tile() : Tile(Tile::Create(Tile::Type::Wall))
{
}

Tile::Tile(Tile::Type type, bool canWalk, char codepoint, ftxui::Color const& color)
    : Entity(codepoint, color), m_type(type), m_canWalk(canWalk)
{
}

Tile Tile::Create(enum class Tile::Type type)
{
    switch (type)
    {
    case Type::Floor:
        return Tile{type, true, '.', ftxui::Color{50, 50, 150}};
    case Type::Wall:
        return Tile{type, false, '#', ftxui::Color::CadetBlue};
    case Type::Void:
        return Tile{type, false, ' ', ftxui::Color::Black};
    default:
        throw std::invalid_argument{"Unknown Tile type"};
    }
}

bool Tile::CanWalk() const
{
    return m_canWalk;
}

Tile::Type Tile::GetType() const
{
    return m_type;
}

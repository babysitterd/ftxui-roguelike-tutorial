#include "Item.hpp"

#include "Color.hpp"

#include <fmt/core.h>

#include <stdexcept>

Item::Item(Type type, Point const& point, char codepoint, ftxui::Color const& color, Effect effect)
    : Entity(codepoint, color), m_type(type), m_point(point), m_effect(std::move(effect))
{
}

Item Item::Create(Item::Type type, Point const& point)
{
    switch (type)
    {
    case Type::HealthPotion:
        return Item{type, point, '!', ftxui::Color(127, 0, 255), [](auto& player, auto& log) {
                        auto const recovered = player.m_fighter.Heal(4);
                        if (recovered > 0)
                        {
                            log.Add(
                                fmt::format("You consume the HEALING POTION, and recover {} HP!",
                                            recovered),
                                Color::HealthRecovered);
                        }
                        else
                        {
                            throw std::logic_error{"Your health is already full."};
                        }
                    }};
    default:
        throw std::invalid_argument{"Unknown Item type"};
    }
}

std::string Item::Name() const
{
    switch (m_type)
    {
    case Type::HealthPotion:
        return "HEALING POTION";
    default:
        throw std::invalid_argument{"Unknown Item type"};
    }
}

#include "Item.hpp"

#include "Color.hpp"
#include "World.hpp"

#include <fmt/core.h>

#include <ranges>
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
        return Item{type, point, '!', ftxui::Color(127, 0, 255), [](auto& world) {
                        auto const recovered = world.m_player.m_fighter.Heal(4);
                        if (recovered > 0)
                        {
                            world.m_messages.Add(
                                fmt::format("You consume the HEALING POTION, and recover {} HP!",
                                            recovered),
                                Color::HealthRecovered);
                        }
                        else
                        {
                            throw std::logic_error{"Your health is already full."};
                        }
                    }};
    case Type::LightningScroll:
        return Item{
            type, point, '~', ftxui::Color(255, 255, 0), [](auto& world) {
                auto range = world.m_actors | std::views::filter([&world](auto const& x) {
                                 return world.m_map.IsLit(x.m_point) && !x.IsDead();
                             });

                if (range.empty())
                {
                    throw std::logic_error("No enemy is close enough to strike.");
                }

                // pick the closest one
                auto distanceToPlayer = [player = world.m_player.m_point](Point const& other) {
                    return std::sqrt(std::pow(static_cast<double>(player.x - other.x), 2.) +
                                     std::pow(static_cast<double>(player.y - other.y), 2.));
                };
                auto it =
                    std::ranges::min_element(range, [&distanceToPlayer](Actor& lhs, Actor& rhs) {
                        return distanceToPlayer(lhs.m_point) < distanceToPlayer(rhs.m_point);
                    });

                int const limit = 5;
                if (distanceToPlayer(it->m_point) > limit)
                {
                    throw std::logic_error("No enemy is close enough to strike.");
                }

                std::string const description =
                    fmt::format("A lighting bolt strikes the {}", it->Name());
                int const damage = 20;
                world.DealDamage(*it, damage, description);
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
    case Type::LightningScroll:
        return "LIGHTNING SCROLL";
    default:
        throw std::invalid_argument{"Unknown Item type"};
    }
}

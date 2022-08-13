#include "Item.hpp"

#include "Color.hpp"
#include "World.hpp"

#include <fmt/core.h>

#include <range/v3/algorithm.hpp>
#include <range/v3/view/filter.hpp>

#include <stdexcept>

Item::Item(Type type, Point const& point, char codepoint, ftxui::Color const& color, Effect effect,
           bool isTargetSpell, int radius)
    : Entity(codepoint, color), m_type(type), m_point(point), m_effect(std::move(effect)),
      m_isTargetSpell(isTargetSpell), m_radius(radius)
{
}

Item Item::Create(Item::Type type, Point const& point)
{
    switch (type)
    {
    case Type::HealthPotion:
        return Item{type, point, '!', ftxui::Color(127, 0, 255), [](auto& world, auto&) {
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
            type, point, '~', ftxui::Color(255, 255, 0), [](auto& world, auto&) {
                auto range = world.m_actors | ranges::views::filter([&world](auto const& x) {
                                 return world.m_map->IsLit(x.m_point) && !x.IsDead();
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
                auto it = ranges::min_element(range, [&distanceToPlayer](Actor& lhs, Actor& rhs) {
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
    case Type::FireballScroll: {
        int const radius = 2;
        return Item{
            type,
            point,
            '~',
            ftxui::Color(255, 0, 0),
            [radius](auto& world, auto& target) {
                if (!world.m_map->IsLit(target))
                {
                    throw std::logic_error("You cannot target an area that you cannot see.");
                }

                auto inTheArea = [&world, &target, radius](auto const& actor) {
                    auto const dx = std::abs(actor.m_point.x - target.x);
                    auto const dy = std::abs(actor.m_point.y - target.y);
                    return dx <= radius && dy <= radius;
                };

                auto range = world.m_actors |
                             ranges::views::filter([&world, &target, inTheArea](auto const& i) {
                                 return !i.IsDead() && inTheArea(i);
                             });

                bool const playerInTheArea = inTheArea(world.m_player);
                if (range.empty() && !playerInTheArea)
                {
                    throw std::logic_error("No suitable targets is in the area.");
                }

                auto dealDamage = [&world](auto& actor) {
                    std::string const description =
                        fmt::format("A flash of fire burns the {}", actor.Name());
                    int const damage = 12;
                    world.DealDamage(actor, damage, description);
                };

                if (playerInTheArea)
                {
                    dealDamage(world.m_player);
                }

                ranges::for_each(range, dealDamage);
            },
            true,
            radius};
    }
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
    case Type::FireballScroll:
        return "FIREBALL SCROLL";
    default:
        throw std::invalid_argument{"Unknown Item type"};
    }
}

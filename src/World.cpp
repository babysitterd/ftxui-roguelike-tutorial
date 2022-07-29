#include "World.hpp"

#include "NaiveMapGenerator.hpp"

#include <fmt/core.h>

#include <algorithm>
#include <array>

namespace
{

constexpr int RoomMaxSize = 10;
constexpr int RoomMinSize = 6;
constexpr int MaxRooms = 30;
constexpr int MaxMonstersPerRoom = 2;

namespace Color
{

auto const PlayerAttack = ftxui::Color(0xE0, 0xE0, 0xE0);
auto const EnemyAttack = ftxui::Color(0xFF, 0xC0, 0xC0);
auto const PlayerDie = ftxui::Color(0xFF, 0x30, 0x30);
auto const EnemyDie = ftxui::Color(0xFF, 0xA0, 0x30);
auto const WelcomeText = ftxui::Color(0x20, 0xA0, 0xFF);
auto const BarText = ftxui::Color::White;
auto const BarFilled = ftxui::Color(0x0, 0x60, 0x0);
auto const BarEmpty = ftxui::Color(0x40, 0x10, 0x10);

} // namespace Color

template <class T> auto FindActorAt(T& actors, Point const& point)
{
    return std::find_if(actors.begin(), actors.end(),
                        [&point](Actor const& x) { return x.m_point == point; });
}

Point NextStep(Point const& from, Point const& to)
{
    Point result(from);

    int const dx = std::abs(to.x - from.x);
    int const sx = from.x < to.x ? 1 : -1;
    int const dy = (-1) * std::abs(to.y - from.y);
    int const sy = from.y < to.y ? 1 : -1;
    int error = dx + dy;

    if (2 * error >= dy)
    {
        error += dy;
        result.x += sx;
    }

    if (2 * error <= dx)
    {
        error += dx;
        result.y += sy;
    }

    return result;
}

} // namespace

World::World(int mapWidth, int mapHeight, int fovRadius)
    : m_rng{}, m_generator(std::make_unique<NaiveMapGenerator>(mapWidth, mapHeight, RoomMaxSize,
                                                               RoomMinSize, MaxRooms, m_rng)),
      m_map(mapWidth, mapHeight, fovRadius, *m_generator),
      m_player(Actor::Create(Actor::Type::Player, m_map.m_rooms.front().Center()))
{
    m_messages.Add("WELCOME TO THE UNDERWORLD, TRAVELER. LET THE GAME BEGIN!", Color::WelcomeText);

    m_map.LineOfSight(m_player.m_point);
    // generate monsters
    for (auto const& room : m_map.m_rooms)
    {
        auto const number = m_rng.RandomInt(0, MaxMonstersPerRoom);
        for (int i = 0; i < number; ++i)
        {
            Point whereTo;
            do
            {
                whereTo = Point(m_rng.RandomInt(room.m_northWest.x + 1, room.m_southEast.x - 1),
                                m_rng.RandomInt(room.m_northWest.y + 1, room.m_southEast.y - 1));
            } while (FindActorAt(m_actors, whereTo) != m_actors.end());

            if (m_rng.RandomInt(0, 100) < 80)
            {
                m_actors.push_back(Actor::Create(Actor::Type::Orc, whereTo));
            }
            else
            {
                m_actors.push_back(Actor::Create(Actor::Type::Troll, whereTo));
            }
        }
    }
}

ftxui::Element World::Render() const
{
    ftxui::Elements world;
    world.reserve(m_map.m_height);
    for (int i = 0; i < m_map.m_height; ++i)
    {
        ftxui::Elements row;
        row.reserve(m_map.m_width + 1);
        auto const line = (i < 10 ? " " : "") + std::to_string(i);
        row.push_back(ftxui::text(std::string{line}) | ftxui::color(ftxui::Color::GrayLight));
        for (int j = 0; j < m_map.m_width; ++j)
        {
            Point const current{j, i};
            bool const isLit = m_map.m_light[j + i * m_map.m_width];
            if (current == m_player.m_point)
            {
                row.push_back(m_player.Render());
            }
            else if (auto it = FindActorAt(m_actors, current); it != m_actors.end() && isLit)
            {
                row.push_back(it->Render());
            }
            else
            {
                row.push_back(m_map.Render(current));
            }
        }
        world.push_back(ftxui::hbox(row));
    }

    // stats pane
    ftxui::Elements stats(m_messages.m_data.size(), ftxui::text(std::string("")));
    // lifebar
    std::string const label =
        fmt::format("HP: {:>3}/{:<3}", m_player.m_fighter.m_hpCurrent, m_player.m_fighter.m_hpFull);
    auto lifebar = ftxui::hbox(ftxui::Elements{
        ftxui::text(label) | ftxui::color(Color::BarText),
        ftxui::text(std::string(m_player.m_fighter.m_hpCurrent, ' ')) |
            ftxui::bgcolor(Color::BarFilled),
        ftxui::text(
            std::string(m_player.m_fighter.m_hpFull - m_player.m_fighter.m_hpCurrent, ' ')) |
            ftxui::bgcolor(Color::BarEmpty)

    });
    stats[0] = lifebar;

    ftxui::Elements all;
    all.push_back(ftxui::border(ftxui::vbox(world)));
    all.push_back(ftxui::hbox(ftxui::Elements{ftxui::border(ftxui::vbox(stats)),
                                              ftxui::border(m_messages.Render()) | ftxui::flex}));
    return ftxui::vbox(all);
}

bool World::EventHandler(ftxui::Event const& event)
{
    std::array<ftxui::Event, 5> const actions{ftxui::Event::ArrowUp, ftxui::Event::ArrowRight,
                                              ftxui::Event::ArrowDown, ftxui::Event::ArrowLeft,
                                              ftxui::Event::Character(' ') /* wait */};

    if (std::find(actions.begin(), actions.end(), event) != actions.end())
    {
        if (m_player.IsDead())
        {
            return false;
        }

        // player makes a move
        if (event != ftxui::Event::Character(' '))
        {
            auto const previous = m_player.m_point;

            if (event == ftxui::Event::ArrowUp)
            {
                --m_player.m_point.y;
            }

            if (event == ftxui::Event::ArrowRight)
            {
                ++m_player.m_point.x;
            }

            if (event == ftxui::Event::ArrowDown)
            {
                ++m_player.m_point.y;
            }

            if (event == ftxui::Event::ArrowLeft)
            {
                --m_player.m_point.x;
            }

            auto toInteract = FindActorAt(m_actors, m_player.m_point);
            auto const isAlive = toInteract != m_actors.end() && !toInteract->IsDead();
            if (isAlive)
            {
                Interact(m_player, *toInteract);
            }

            // rewind if the move was illigal
            if (m_map.IsOutOfBounds(m_player.m_point) || !m_map.At(m_player.m_point).CanWalk() ||
                isAlive)
            {
                m_player.m_point = previous;
            }
            else
            {
                m_map.LineOfSight(m_player.m_point);
            }
        }

        // world takes turn
        for (auto& actor : m_actors)
        {
            if (actor.IsDead())
            {
                continue;
            }

            auto const delta = std::abs(actor.m_point.x - m_player.m_point.x) +
                               std::abs(actor.m_point.y - m_player.m_point.y);

            if (delta == 1)
            {
                Interact(actor, m_player);
            }
            else if (m_map.m_light[actor.m_point.x + actor.m_point.y * m_map.m_width])
            {
                actor.m_point = NextStep(actor.m_point, m_player.m_point);
            }
        }

        if (m_player.IsDead())
        {
            m_messages.Add("GAME OVER ☠", ftxui::Color::Red1);
        }

        return true;
    }

    return false;
}

void World::Interact(Actor& first, Actor& second)
{
    int const damage = first.m_fighter.m_power - second.m_fighter.m_defense;

    std::string const description = fmt::format("{} attacks {}", first.Name(), second.Name());
    if (damage > 0)
    {
        second.m_fighter.SetHitpoints(second.m_fighter.m_hpCurrent - damage);
        if (second.m_fighter.m_hpCurrent == 0)
        {
            m_messages.Add(fmt::format("{} for {} hit points. {} is dead!", description, damage,
                                       second.Name()),
                           second.m_type == Actor::Type::Player ? Color::PlayerDie
                                                                : Color::EnemyDie);
            second.Die();
        }
        else
        {
            m_messages.Add(fmt::format("{} for {} hit points. ({}/{})", description, damage,
                                       second.m_fighter.m_hpCurrent, second.m_fighter.m_hpFull),
                           first.m_type == Actor::Type::Player ? Color::PlayerAttack
                                                               : Color::EnemyAttack);
        }
    }
    else
    {
        m_messages.Add(fmt::format("{} but does no damage. ({}/{})", description,
                                   second.m_fighter.m_hpCurrent, second.m_fighter.m_hpFull),
                       first.m_type == Actor::Type::Player ? Color::PlayerAttack
                                                           : Color::EnemyAttack);
    }
}

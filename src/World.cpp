#include "World.hpp"

#include "NaiveMapGenerator.hpp"

#include <ftxui/component/mouse.hpp>

#include <fmt/core.h>

#include <algorithm>
#include <array>
#include <cctype>

using namespace std::string_literals;

namespace
{

constexpr int RoomMaxSize = 10;
constexpr int RoomMinSize = 6;
constexpr int MaxRooms = 30;
constexpr int MaxMonstersPerRoom = 2;
constexpr int MaxPotionsPerRoom = 2;
constexpr int MaxInventoryCapacity = 10;

template <class Iterator> auto FindAt(Iterator from, Iterator to, Point const& point)
{
    return std::find_if(from, to, [&point](auto const& x) { return x.m_point == point; });
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
        result.x += sx;
    }

    if (2 * error <= dx)
    {
        result.y += sy;
    }

    return result;
}

auto RenderHelp()
{
    ftxui::Elements help;
    auto add = [&help](std::string const& str) {
        help.push_back(ftxui::text(str) | ftxui::color(Color::ControlsText));
    };

    add(" Hover over the monsters with your mosue to get more information about those");
    add(" Use arrow keys to move around the dungeon, attack monsters and interact with NPC");
    add(" Use SPACE to skip the turn and let the monster come to you without getting hit");
    add(" Use i to open inventory and use items, d to drop stuff on the ground and g to pick up");
    add(" Use Enter while on > to go downstairs and q to give up and cowardly leave the dungeon");

    return ftxui::window(ftxui::text("[  Controls  ]") | ftxui::center, ftxui::vbox(help));
}

} // namespace

World::World(int mapWidth, int mapHeight, int fovRadius)
    : m_rng{}, m_generator(std::make_unique<NaiveMapGenerator>(mapWidth, mapHeight, RoomMaxSize,
                                                               RoomMinSize, MaxRooms, m_rng)),
      m_player({0, 0})
{
    GenerateLevel(mapWidth, mapHeight, fovRadius);

    m_messages.Add("WELCOME TO THE UNDERWORLD, TRAVELER. LET THE GAME BEGIN!", Color::WelcomeText);
    m_messages.Add("Press v to open an ancient book of knowledge...", Color::HintText);
}

void World::GenerateLevel(int mapWidth, int mapHeight, int fovRadius)
{
    m_map = std::make_unique<FovMap>(mapWidth, mapHeight, fovRadius, *m_generator);
    m_player.m_point = m_map->m_rooms.front().Center();
    m_map->LineOfSight(m_player.m_point);

    GenerateMonsters();
    GenerateItems();

    ++m_level;
}

ftxui::Element World::Render() const
{
    if (m_current == Mode::Messages)
    {
        auto history = ftxui::window(
            ftxui::text("[  Message history  ]") | ftxui::center,
            m_messages.RenderAll(m_focusedMessage) | ftxui::vscroll_indicator | ftxui::frame |
                ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, m_map->m_height - 1) |
                ftxui::size(ftxui::WIDTH, ftxui::EQUAL, m_map->m_width - 1));

        ftxui::Elements all;
        all.push_back(history);
        all.push_back(RenderHelp());
        return ftxui::vbox(all);
    }

    ftxui::Element mainArea;
    if (m_current == Mode::Game || m_current == Mode::SelectingTarget)
    {
        ftxui::Elements field;
        field.reserve(m_map->m_height);
        for (int i = 0; i < m_map->m_height; ++i)
        {
            ftxui::Elements row;
            row.reserve(m_map->m_width);
            for (int j = 0; j < m_map->m_width; ++j)
            {
                Point const current{j, i};
                bool const isLit = m_map->IsLit({j, i});
                auto dead = std::partition_point(m_actors.begin(), m_actors.end(),
                                                 [](auto& x) { return !x.IsDead(); });
                // Render order:
                // 1. Player
                if (current == m_player.m_point)
                {
                    row.push_back(m_player.Render());
                }
                // 2. Living actors
                else if (auto it = FindAt(m_actors.begin(), dead, current); it != dead && isLit)
                {
                    row.push_back(it->Render());
                }
                // 3. Items
                else if (auto it = FindAt(m_items.begin(), m_items.end(), current);
                         it != m_items.end() && isLit)
                {
                    row.push_back(it->Render());
                }
                // 4. Corpses
                else if (auto it = FindAt(dead, m_actors.end(), current);
                         it != m_actors.end() && isLit)
                {
                    row.push_back(it->Render());
                }
                // 5. Map terrain
                else
                {
                    row.push_back(m_map->Render(current));
                }

                if (m_current == Mode::SelectingTarget)
                {
                    auto const dx = std::abs(current.x - m_mouse.x);
                    auto const dy = std::abs(current.y - m_mouse.y);

                    if (dx <= m_inventory[m_itemInUse].m_radius &&
                        dy <= m_inventory[m_itemInUse].m_radius)
                    {
                        row.back() = row.back() | ftxui::bgcolor(ftxui::Color::DarkRedBis);
                    }
                }
            }
            field.push_back(ftxui::hbox(row));
        }
        mainArea = ftxui::border(ftxui::vbox(field));
    }
    else if (m_current == Mode::InventoryUse || m_current == Mode::InventoryDrop)
    {
        ftxui::Elements all;
        char letter = 'a';
        for (auto const& item : m_inventory)
        {
            all.push_back(ftxui::text(fmt::format("{}. {}", letter++, item.Name())) |
                          ftxui::color(Color::ControlsText));
        }

        if (all.empty())
        {
            all.push_back(ftxui::text("( EMPTY )") | ftxui::color(Color::HintText));
        }

        std::string const header = fmt::format("[ Select an item to {} ]",
                                               m_current == Mode::InventoryUse ? "use" : "drop");
        auto inventory = ftxui::window(ftxui::text(header) | ftxui::center, ftxui::vbox(all)) |
                         ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, m_map->m_height / 2) |
                         ftxui::size(ftxui::WIDTH, ftxui::EQUAL, m_map->m_width / 2) |
                         ftxui::center;

        mainArea = ftxui::border(inventory);
    }

    // character information pane
    ftxui::Elements info;
    info.push_back(ftxui::text("-= HERO =-") | ftxui::center);
    info.push_back(ftxui::text(fmt::format("LEVEL: {}", m_player.m_level)));
    info.push_back(ftxui::text(fmt::format("ATTACK: {}", m_player.m_fighter.m_power)));
    info.push_back(ftxui::text(fmt::format("DEFENCE: {}", m_player.m_fighter.m_defense)));
    info.push_back(
        ftxui::text(fmt::format("XP: {}/{}", m_player.m_currentXp, m_player.XpToNextLevel())));
    info.push_back(ftxui::border(
        ftxui::gauge(static_cast<float>(m_player.m_currentXp - m_player.XpToPreviousLevel()) /
                     (m_player.XpToNextLevel() - m_player.XpToPreviousLevel()))));

    // stats pane
    constexpr std::size_t displayed = 5;
    ftxui::Elements stats(displayed, ftxui::text(""s));
    // lifebar
    auto generateLifebar = [this](Actor const& actor) {
        std::string const label =
            fmt::format("HP: {:>3}/{:<3}", actor.m_fighter.m_hpCurrent, actor.m_fighter.m_hpFull);
        return ftxui::hbox(ftxui::Elements{
            ftxui::text(label) | ftxui::color(Color::BarText),
            ftxui::text(std::string(actor.m_fighter.m_hpCurrent, ' ')) |
                ftxui::bgcolor(Color::BarFilled),
            ftxui::text(std::string(actor.m_fighter.m_hpFull - actor.m_fighter.m_hpCurrent, ' ')) |
                ftxui::bgcolor(Color::BarEmpty)});
    };
    stats[0] = ftxui::text(m_player.Name() + ":") | ftxui::color(ftxui::Color::Green);
    stats[1] = generateLifebar(m_player);
    if (m_map->IsLit(m_mouse))
    {
        if (auto it = FindAt(m_actors.begin(), m_actors.end(), m_mouse); it != m_actors.end())
        {
            stats[2] = ftxui::text(it->Name()) | ftxui::color(ftxui::Color::Red);
            stats[3] = generateLifebar(*it);
        }
    }
    stats[4] =
        ftxui::text(fmt::format("Dungeon level: {}", m_level)) | ftxui::color(Color::WelcomeText);

    ftxui::Elements plus;
    plus.push_back(mainArea | ftxui::size(ftxui::HEIGHT, ftxui::EQUAL, m_map->m_height + 1) |
                   ftxui::size(ftxui::WIDTH, ftxui::EQUAL, m_map->m_width + 1));
    plus.push_back(ftxui::border(ftxui::vbox(info)) | ftxui::size(ftxui::WIDTH, ftxui::EQUAL, 15));

    ftxui::Elements all;
    all.push_back(ftxui::hbox(plus));
    all.push_back(
        ftxui::hbox(ftxui::Elements{ftxui::border(ftxui::vbox(stats)),
                                    ftxui::border(m_messages.Render(displayed)) | ftxui::flex}));
    return ftxui::vbox(all);
}

bool World::EventHandler(ftxui::Event& event)
{
    if (event.is_mouse())
    {
        m_mouse.x = (event.mouse().x - 1);
        m_mouse.y = (event.mouse().y - 1);

        if (m_current == Mode::SelectingTarget && event.mouse().button == ftxui::Mouse::Left &&
            event.mouse().motion == ftxui::Mouse::Released)
        {
            UseItem(m_itemInUse, m_mouse);
        }

        return false;
    }

    // return back to the game from any mode
    if (event == ftxui::Event::Escape && m_current != Mode::Game)
    {
        m_current = Mode::Game;
        return true;
    }

    if (m_current == Mode::Game)
    {
        // Message history tab opens, last message is focused
        if (event == ftxui::Event::Character('v'))
        {
            m_current = Mode::Messages;
            m_focusedMessage = m_messages.m_data.size() - 1;

            return true;
        }
        // Open inventory to use item
        if (event == ftxui::Event::Character('i'))
        {
            m_current = Mode::InventoryUse;
            return true;
        }

        // Open inventory to drop item
        if (event == ftxui::Event::Character('d'))
        {
            m_current = Mode::InventoryDrop;
            return true;
        }
    }

    // Handle inventory mode choice
    if (m_current == Mode::InventoryUse || m_current == Mode::InventoryDrop)
    {
        if (event.is_character())
        {
            std::string const tmp = event.character();
            if (tmp.size() != 1)
            {
                return false;
            }

            std::size_t index = std::tolower(static_cast<unsigned char>(tmp.front())) - int{'a'};
            if (index < m_inventory.size())
            {
                if (m_current == Mode::InventoryUse)
                {
                    if (m_inventory[index].m_isTargetSpell)
                    {
                        m_itemInUse = index;
                        m_current = Mode::SelectingTarget;
                    }
                    else
                    {
                        UseItem(index, Point{});
                    }
                }
                else
                {
                    m_messages.Add(fmt::format("You dropped the {}.", m_inventory[index].Name()),
                                   Color::HintText);
                    m_items.push_back(m_inventory[index]);
                    m_items.back().m_point = m_player.m_point;
                    m_inventory.erase(m_inventory.begin() + index);
                    m_current = Mode::Game;
                }
            }
            else
            {
                m_messages.Add("Invalid entry.", Color::HintText);
            }
        }
    }

    if (m_current == Mode::Messages)
    {
        if (event == ftxui::Event::ArrowUp && m_focusedMessage != 0)
        {
            --m_focusedMessage;
        }

        if (event == ftxui::Event::ArrowDown && m_focusedMessage != m_messages.m_data.size() - 1)
        {
            ++m_focusedMessage;
        }
    }

    std::array<ftxui::Event, 7> const actions{ftxui::Event::ArrowUp,
                                              ftxui::Event::ArrowRight,
                                              ftxui::Event::ArrowDown,
                                              ftxui::Event::ArrowLeft,
                                              ftxui::Event::Return, /* downstairs */
                                              ftxui::Event::Character(' ') /* wait */,
                                              ftxui::Event::Character('g') /* pick up */};

    if (m_current == Mode::Game &&
        std::find(actions.begin(), actions.end(), event) != actions.end())
    {
        if (m_player.IsDead())
        {
            return false;
        }

        if (event == ftxui::Event::Return)
        {
            if (m_map->At(m_player.m_point).GetType() == Tile::Type::Downstairs)
            {
                GenerateLevel(m_map->m_width, m_map->m_height, m_map->m_radius);
                m_messages.Add("You descend the staircase.", Color::DescentText);
            }
            else
            {
                m_messages.Add("There are no stairs here.", Color::HintText);
            }
            return true;
        }

        // player makes a move
        if (event == ftxui::Event::Character('g'))
        {
            if (auto toPickUp = FindAt(m_items.begin(), m_items.end(), m_player.m_point);
                toPickUp != m_items.end())
            {
                if (m_inventory.size() < MaxInventoryCapacity)
                {
                    m_messages.Add(fmt::format("You picked up the {}!", toPickUp->Name()),
                                   Color::BarText);
                    m_inventory.push_back(*toPickUp);
                    m_items.erase(toPickUp);
                }
                else
                {
                    m_messages.Add("Your inventory is full.", Color::HintText);
                }
            }
            else
            {
                m_messages.Add("There is nothing to pick up", Color::HintText);
            }
        }
        else if (event != ftxui::Event::Character(' '))
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

            auto toInteract = FindAt(m_actors.begin(), m_actors.end(), m_player.m_point);
            auto const isAlive = toInteract != m_actors.end() && !toInteract->IsDead();
            if (isAlive)
            {
                MeleeAttack(m_player, *toInteract);
            }

            // rewind if the move was illigal
            if (m_map->IsOutOfBounds(m_player.m_point) || !m_map->At(m_player.m_point).CanWalk() ||
                isAlive)
            {
                m_player.m_point = previous;
            }
            else
            {
                m_map->LineOfSight(m_player.m_point);
            }
        }

        // world takes turn
        for (auto& actor : m_actors)
        {
            if (actor.IsDead())
            {
                continue;
            }

            auto const dx = std::abs(actor.m_point.x - m_player.m_point.x);
            auto const dy = std::abs(actor.m_point.y - m_player.m_point.y);

            // monster will attack if placed adjacent to player
            if (dx + dy == 1 || (dx == 1 && dy == 1))
            {
                MeleeAttack(actor, m_player);
            }
            else if (m_map->IsLit(actor.m_point))
            {
                auto const next = NextStep(actor.m_point, m_player.m_point);
                if (m_map->At(next).CanWalk())
                {
                    actor.m_point = next;
                }
            }
        }
        // to render living actors first
        std::partition(m_actors.begin(), m_actors.end(), [](auto& x) { return !x.IsDead(); });

        if (m_player.IsDead())
        {
            m_messages.Add("GAME OVER ☠", ftxui::Color::Red1);
        }

        return true;
    }

    return false;
}

void World::MeleeAttack(Actor& first, Actor& second)
{
    if (second.IsDead())
    {
        return;
    }

    int const damage = first.m_fighter.m_power - second.m_fighter.m_defense;

    std::string const description = fmt::format("{} attacks {}", first.Name(), second.Name());

    DealDamage(second, damage, description);
}

void World::DealDamage(Actor& receiver, int damage, std::string const& description)
{
    if (damage > 0)
    {
        receiver.m_fighter.TakeDamage(damage);
        if (receiver.m_fighter.m_hpCurrent == 0)
        {
            m_messages.Add(
                fmt::format("{} for {} damage. {} is dead!", description, damage, receiver.Name()),
                receiver.m_type == Actor::Type::Player ? Color::PlayerDie : Color::EnemyDie);
            receiver.Die();

            if (receiver.m_type != Actor::Type::Player)
            {
                m_messages.Add(fmt::format("You gain {} experience points.", receiver.m_xpGiven),
                               Color::PlayerAttack);
                if (m_player.Gain(receiver.m_xpGiven))
                {
                    m_messages.Add(fmt::format("You advance to level {}!", m_player.m_level),
                                   Color::HealthRecovered);
                }
            }
        }
        else
        {
            m_messages.Add(fmt::format("{} for {} damage. ({}/{})", description, damage,
                                       receiver.m_fighter.m_hpCurrent, receiver.m_fighter.m_hpFull),
                           receiver.m_type == Actor::Type::Player ? Color::EnemyAttack
                                                                  : Color::PlayerAttack);
        }
    }
    else
    {
        m_messages.Add(fmt::format("{} but does no damage. ({}/{})", description,
                                   receiver.m_fighter.m_hpCurrent, receiver.m_fighter.m_hpFull),
                       receiver.m_type == Actor::Type::Player ? Color::EnemyAttack
                                                              : Color::PlayerAttack);
    }
}

void World::GenerateMonsters()
{
    m_actors.clear();

    for (auto const& room : m_map->m_rooms)
    {
        auto const number = m_rng.RandomInt(0, MaxMonstersPerRoom);
        for (int i = 0; i < number; ++i)
        {
            Point whereTo;
            do
            {
                whereTo = Point(m_rng.RandomInt(room.m_northWest.x + 1, room.m_southEast.x - 1),
                                m_rng.RandomInt(room.m_northWest.y + 1, room.m_southEast.y - 1));
            } while (FindAt(m_actors.begin(), m_actors.end(), whereTo) != m_actors.end());

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

void World::GenerateItems()
{
    m_items.clear();

    for (auto const& room : m_map->m_rooms)
    {
        auto const number = m_rng.RandomInt(0, MaxPotionsPerRoom);
        for (int i = 0; i < number; ++i)
        {
            Point whereTo;
            do
            {
                whereTo = Point(m_rng.RandomInt(room.m_northWest.x + 1, room.m_southEast.x - 1),
                                m_rng.RandomInt(room.m_northWest.y + 1, room.m_southEast.y - 1));
            } while (FindAt(m_actors.begin(), m_actors.end(), whereTo) != m_actors.end() &&
                     FindAt(m_items.begin(), m_items.end(), whereTo) != m_items.end());

            auto const dice = m_rng.RandomInt(0, 100);
            if (dice < 60)
            {
                m_items.push_back(Item::Create(Item::Type::HealthPotion, whereTo));
            }
            else if (dice < 80)
            {
                m_items.push_back(Item::Create(Item::Type::LightningScroll, whereTo));
            }
            else
            {
                m_items.push_back(Item::Create(Item::Type::FireballScroll, whereTo));
            }
        }
    }
}

void World::UseItem(std::size_t index, Point const& target)
{
    try
    {
        m_inventory[index].m_effect(*this, target);
        m_inventory.erase(m_inventory.begin() + index);
        m_current = Mode::Game;
    }
    catch (std::logic_error& e)
    {
        m_messages.Add(e.what(), Color::HintText);
    }
}

#include "World.hpp"

#include "NaiveMapGenerator.hpp"

#include <algorithm>
#include <array>

namespace
{

constexpr int RoomMaxSize = 10;
constexpr int RoomMinSize = 6;
constexpr int MaxRooms = 30;
constexpr int MaxMonstersPerRoom = 2;

} // namespace

World::World(int mapWidth, int mapHeight, int fovRadius)
    : m_rng{}, m_generator(std::make_unique<NaiveMapGenerator>(mapWidth, mapHeight, RoomMaxSize,
                                                               RoomMinSize, MaxRooms, m_rng)),
      m_map(mapWidth, mapHeight, fovRadius, *m_generator),
      m_player(Actor::Create(Actor::Type::Player, m_map.m_rooms.front().Center()))
{
    m_messages.Add("WELCOME TO THE UNDERWORLD, TRAVELER. LET THE GAME BEGIN!");

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
            } while (m_actors.contains(whereTo));

            if (m_rng.RandomInt(0, 100) < 80)
            {
                m_actors.insert(Actor::Create(Actor::Type::Orc, whereTo));
            }
            else
            {
                m_actors.insert(Actor::Create(Actor::Type::Troll, whereTo));
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
            else if (auto it = m_actors.find(current); it != m_actors.end() && isLit)
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

    ftxui::Elements all;
    all.push_back(ftxui::border(ftxui::vbox(world)));
    all.push_back(ftxui::border(m_messages.Render()));
    return ftxui::vbox(all);
}

bool World::EventHandler(ftxui::Event const& event)
{
    std::array<ftxui::Event, 4> const directions{ftxui::Event::ArrowUp, ftxui::Event::ArrowRight,
                                                 ftxui::Event::ArrowDown, ftxui::Event::ArrowLeft};

    if (std::find(directions.begin(), directions.end(), event) != directions.end())
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

        auto toInteract = m_actors.find(m_player.m_point);
        if (toInteract != m_actors.end())
        {
            Interact(m_player, *toInteract);
        }

        // rewind if the move was illigal
        if (m_map.IsOutOfBounds(m_player.m_point) || !m_map.At(m_player.m_point).CanWalk() ||
            toInteract != m_actors.end())
        {
            m_player.m_point = previous;
        }
        else
        {
            m_map.LineOfSight(m_player.m_point);
        }

        return true;
    }

    return false;
}

void World::Interact(Actor const& player, Actor const& other)
{
    m_messages.Add("You kick the " + other.Name() + ", much to its annoyance!");
}

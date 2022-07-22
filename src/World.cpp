#include "World.hpp"

#include "NaiveMapGenerator.hpp"

namespace
{

constexpr int RoomMaxSize = 10;
constexpr int RoomMinSize = 6;
constexpr int MaxRooms = 30;

} // namespace

World::World(int mapWidth, int mapHeight, int fovRadius)
    : m_rng{}, m_generator(std::make_unique<NaiveMapGenerator>(mapWidth, mapHeight, RoomMaxSize,
                                                               RoomMinSize, MaxRooms, m_rng)),
      m_map(mapWidth, mapHeight, fovRadius, *m_generator),
      m_player(m_map.m_rooms.front().Center(), '@', ftxui::Color::White)
{
    m_map.LineOfSight(m_player.m_point);
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
            if (current == m_player.m_point)
            {
                row.push_back(m_player.Render());
            }
            else if (auto it = m_actors.find(current); it != m_actors.end())
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
    return ftxui::vbox(world);
}

bool World::EventHandler(ftxui::Event const& event)
{
    auto rewind = [this, previous = m_player.m_point]() {
        if (m_map.IsOutOfBounds(m_player.m_point) || !m_map.At(m_player.m_point).CanWalk())
        {
            m_player.m_point = previous;
        }
        else
        {
            m_map.LineOfSight(m_player.m_point);
        }
    };

    if (event == ftxui::Event::ArrowUp)
    {
        --m_player.m_point.y;
        rewind();
        return true;
    }

    if (event == ftxui::Event::ArrowRight)
    {
        ++m_player.m_point.x;
        rewind();
        return true;
    }

    if (event == ftxui::Event::ArrowDown)
    {
        ++m_player.m_point.y;
        rewind();
        return true;
    }

    if (event == ftxui::Event::ArrowLeft)
    {
        --m_player.m_point.x;
        rewind();
        return true;
    }

    return false;
}

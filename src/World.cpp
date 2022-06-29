#include "World.hpp"

World::World(Point const& player_position, int mapWidth, int mapHeight) :
    m_player(player_position, '@', ftxui::Color::White),
    m_map(mapWidth, mapHeight)
{
    m_actors.emplace(Point{60, 13}, '@', ftxui::Color::Yellow);
}

ftxui::Element World::render() const
{
    ftxui::Elements world;
    world.reserve(m_map.m_height);
    for (int i = 0; i < m_map.m_height; ++i)
    {
        ftxui::Elements row;
        row.reserve(m_map.m_width);
        for (int j = 0; j < m_map.m_width; ++j)
        {
            Point const current{j, i};
            if (current == m_player.m_point)
            {
                row.push_back(m_player.render());
            }
            else if (auto it = m_actors.find(current);
                     it != m_actors.end())
            {
                row.push_back(it->render());
            }
            else
            {
                row.push_back(m_map.render(current));
            }
        }
        world.push_back(ftxui::hbox(row));
    }
    return ftxui::vbox(world);
}

bool World::EventHandler(ftxui::Event const& event)
{
    auto rewind = [this, previous = m_player.m_point]()
    {
        if (m_map.isWall(m_player.m_point))
        {
            m_player.m_point = previous;
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

#pragma once

#include "Actor.hpp"
#include "FovMap.hpp"
#include "Point.hpp"

#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <set>

struct World
{
    World(int mapWidth, int mapHeight, int fovRadius);

    ftxui::Element Render() const;

    bool EventHandler(ftxui::Event const& event);

    FovMap m_map;
    Actor m_player;
    std::set<Actor, Compare> m_actors;
};

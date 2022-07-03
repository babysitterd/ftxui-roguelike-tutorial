#pragma once

#include "Actor.hpp"
#include "Map.hpp"
#include "Point.hpp"

#include <ftxui/component/event.hpp>
#include <ftxui/dom/elements.hpp>

#include <set>

struct World
{
    World(int mapWidth, int mapHeight);

    ftxui::Element Render() const;

    bool EventHandler(ftxui::Event const& event);

    Map m_map;
    Actor m_player;
    std::set<Actor, Compare> m_actors;
};
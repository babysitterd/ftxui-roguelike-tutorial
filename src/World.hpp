#pragma once

#include "Actor.hpp"
#include "Point.hpp"
#include "Map.hpp"

#include <ftxui/dom/elements.hpp>
#include <ftxui/component/event.hpp>

#include <set>

struct World
{
    World(Point const& playerPosition, int mapWidth, int mapHeight);

    ftxui::Element render() const;

    bool EventHandler(ftxui::Event const& event);

    Actor m_player;
    Map m_map;
    std::set<Actor, Compare> m_actors;
};
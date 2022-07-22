#pragma once

#include "Actor.hpp"
#include "FovMap.hpp"
#include "MapGeneratorBase.hpp"
#include "Point.hpp"
#include "RNG.hpp"

#include <ftxui/component/event.hpp>

#include <ftxui/dom/elements.hpp>

#include <set>

struct World
{
    World(int mapWidth, int mapHeight, int fovRadius);

    ftxui::Element Render() const;

    bool EventHandler(ftxui::Event const& event);

    RNG m_rng;
    std::unique_ptr<MapGeneratorBase> m_generator;
    FovMap m_map;
    Actor m_player;
    std::set<Actor, Compare> m_actors;
};

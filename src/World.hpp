#pragma once

#include "Actor.hpp"
#include "FovMap.hpp"
#include "MapGeneratorBase.hpp"
#include "MessageLog.hpp"
#include "Point.hpp"
#include "RNG.hpp"

#include <ftxui/component/event.hpp>

#include <ftxui/dom/elements.hpp>

#include <vector>

enum class Tab
{
    Game,
    Messages
};

class World
{
  public:
    World(int mapWidth, int mapHeight, int fovRadius);

    ftxui::Element Render() const;

    bool EventHandler(ftxui::Event const& event);
    void Interact(Actor& first, Actor& second);

  private:
    RNG m_rng;
    std::unique_ptr<MapGeneratorBase> m_generator;
    FovMap m_map;
    Actor m_player;
    std::vector<Actor> m_actors;
    MessageLog m_messages;
    Tab m_current = Tab::Game;
    std::size_t m_focusedMessage;
};

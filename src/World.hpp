#pragma once

#include "Actor.hpp"
#include "Color.hpp"
#include "FovMap.hpp"
#include "Item.hpp"
#include "MapGeneratorBase.hpp"
#include "MessageLog.hpp"
#include "Player.hpp"
#include "Point.hpp"
#include "RNG.hpp"

#include <ftxui/component/event.hpp>

#include <ftxui/dom/elements.hpp>

#include <vector>

enum class Mode
{
    Game,
    Messages,
    InventoryUse,
    InventoryDrop,
    SelectingTarget
};

class World
{
  public:
    World(int mapWidth, int mapHeight, int fovRadius);

    ftxui::Element Render() const;

    bool EventHandler(ftxui::Event& event);
    void MeleeAttack(Actor& first, Actor& second);
    void DealDamage(Actor& receiver, int damage, std::string const& description);
    void UseItem(std::size_t index, Point const& target);

    void GenerateLevel(int mapWidth, int mapHeight, int fovRadius);
    void GenerateMonsters();
    void GenerateItems();

    RNG m_rng;
    std::unique_ptr<MapGeneratorBase> m_generator;
    Player m_player;
    std::unique_ptr<FovMap> m_map;
    std::vector<Actor> m_actors;
    std::vector<Item> m_items;
    std::vector<Item> m_inventory;
    Mode m_current = Mode::Game;
    MessageLog m_messages;
    std::size_t m_focusedMessage;
    Point m_mouse;
    std::size_t m_itemInUse;
    int m_level = 0;
};

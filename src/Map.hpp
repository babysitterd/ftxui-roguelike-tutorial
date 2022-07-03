#pragma once

#include "Entity.hpp"
#include "Point.hpp"
#include "Tile.hpp"

#include <ftxui/dom/elements.hpp>

#include <vector>

struct Room
{
    Room(int x, int y, int width, int height)
        : m_northWest{x, y}, m_southEast{x + width, y + height}
    {
    }

    Point Center() const
    {
        return {(m_northWest.x + m_southEast.x) / 2, (m_northWest.y + m_southEast.y) / 2};
    }

    bool Intersects(Room const& other) const
    {
        return m_northWest.x <= other.m_southEast.x && m_southEast.x >= other.m_northWest.x &&
               m_northWest.y <= other.m_southEast.y && m_southEast.y >= other.m_northWest.y;
    }

    Point m_northWest;
    Point m_southEast;
};

#include <random>

struct IDigger
{
    virtual ~IDigger() = default;

    virtual void CarveRoom(Room const& room) = 0;
    virtual void DigHorizontalTunnel(int x1, int x2, int y) = 0;
    virtual void DigVerticalTunnel(int y1, int y2, int x) = 0;
};

class MapGenerator
{
  public:
    MapGenerator(int width, int height,
                 std::random_device::result_type seed = std::random_device{}())
        : m_mapWidth(width), m_mapHeight(height), m_generator(seed)
    {
    }

    std::vector<Room> Generate(int roomMaxSize, int roomMinSize, int maxRooms, IDigger& digger)
    {
        std::vector<Room> rooms;
        for (int i = 0; i < maxRooms; ++i)
        {
            int const roomWidth = RandomInt(roomMinSize, roomMaxSize);
            int const roomHeight = RandomInt(roomMinSize, roomMaxSize);

            int const x = RandomInt(0, m_mapWidth - roomWidth - 1);
            int const y = RandomInt(0, m_mapHeight - roomHeight - 1);

            Room newRoom(x, y, roomWidth, roomHeight);
            bool canInsert = true;
            for (auto const& otherRoom : rooms)
            {
                if (newRoom.Intersects(otherRoom))
                {
                    canInsert = false;
                    break;
                }
            }

            if (canInsert)
            {
                digger.CarveRoom(newRoom);

                // connect to the previous room
                if (!rooms.empty())
                {
                    auto const current = newRoom.Center();
                    auto const previous = rooms.back().Center();

                    if (RandomInt(0, 1) == 1)
                    {
                        digger.DigHorizontalTunnel(previous.x, current.x, previous.y);
                        digger.DigVerticalTunnel(previous.y, current.y, current.x);
                    }
                    else
                    {
                        digger.DigVerticalTunnel(previous.y, current.y, previous.x);
                        digger.DigHorizontalTunnel(previous.x, current.x, current.y);
                    }
                }

                rooms.push_back(std::move(newRoom));
            }
        }

        return rooms;
    }

    int RandomInt(int from, int to)
    {
        std::uniform_int_distribution<> distribution(from, to);
        return distribution(m_generator);
    }

  private:
    int m_mapWidth;
    int m_mapHeight;
    std::mt19937 m_generator;
};

class Map : public IDigger
{
  public:
    Map(int width, int height);

    void CarveRoom(Room const& room) override;
    void DigHorizontalTunnel(int x1, int x2, int y) override;
    void DigVerticalTunnel(int y1, int y2, int x) override;

    Tile const& At(Point const& point) const;
    bool IsOutOfBounds(Point const& point) const;

    ftxui::Element Render(Point const& point) const;

    int m_width;
    int m_height;
    std::vector<Tile> m_tiles;
    std::vector<Room> m_rooms;
};

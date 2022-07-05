#pragma once

#include "Room.hpp"

#include <random>

class MapGenerator
{
  public:
    MapGenerator(int width, int height,
                 std::random_device::result_type seed = std::random_device{}())
        : m_mapWidth(width), m_mapHeight(height), m_generator(seed)
    {
    }

    template <class T>
    std::vector<Room> Generate(int roomMaxSize, int roomMinSize, int maxRooms, T& digger)
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
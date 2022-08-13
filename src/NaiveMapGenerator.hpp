#pragma once

#include "MapGeneratorBase.hpp"

class NaiveMapGenerator : public MapGeneratorBase
{
  public:
    using MapGeneratorBase::MapGeneratorBase;

    std::vector<Room> Generate(IMapMutator& map) override
    {
        std::vector<Room> rooms;
        for (int i = 0; i < m_maxRooms; ++i)
        {
            int const roomWidth = m_rng.RandomInt(m_roomMinSize, m_roomMaxSize);
            int const roomHeight = m_rng.RandomInt(m_roomMinSize, m_roomMaxSize);

            int const x = m_rng.RandomInt(0, m_mapWidth - roomWidth - 1);
            int const y = m_rng.RandomInt(0, m_mapHeight - roomHeight - 1);

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
                map.CarveRoom(newRoom);

                // connect to the previous room
                if (!rooms.empty())
                {
                    auto const current = newRoom.Center();
                    auto const previous = rooms.back().Center();

                    if (m_rng.RandomInt(0, 1) == 1)
                    {
                        map.DigHorizontalTunnel(previous.x, current.x, previous.y);
                        map.DigVerticalTunnel(previous.y, current.y, current.x);
                    }
                    else
                    {
                        map.DigVerticalTunnel(previous.y, current.y, previous.x);
                        map.DigHorizontalTunnel(previous.x, current.x, current.y);
                    }
                }

                rooms.push_back(std::move(newRoom));
            }
        }

        map.DigDownstairs(rooms.back().Center());

        return rooms;
    }
};

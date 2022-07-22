#pragma once

#include "IMapMutator.hpp"
#include "RNG.hpp"
#include "Room.hpp"

#include <vector>

class MapGeneratorBase
{
  public:
    MapGeneratorBase(int width, int height, int roomMaxSize, int roomMinSize, int maxRooms,
                     RNG& rng)
        : m_mapWidth(width), m_mapHeight(height), m_roomMaxSize(roomMaxSize),
          m_roomMinSize(roomMinSize), m_maxRooms(maxRooms), m_rng(rng)
    {
    }
    virtual ~MapGeneratorBase() = default;

    virtual std::vector<Room> Generate(IMapMutator& map) = 0;

  protected:
    int m_mapWidth;
    int m_mapHeight;
    int m_roomMaxSize;
    int m_roomMinSize;
    int m_maxRooms;
    RNG& m_rng;
};

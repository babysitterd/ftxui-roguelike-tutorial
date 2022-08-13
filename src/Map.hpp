#pragma once

#include "Entity.hpp"
#include "IMapMutator.hpp"
#include "MapGeneratorBase.hpp"
#include "Point.hpp"
#include "Room.hpp"
#include "Tile.hpp"

#include <ftxui/dom/elements.hpp>

#include <iosfwd>
#include <string>
#include <vector>

class Map : public IMapMutator
{
  public:
    Map(int width, int height, MapGeneratorBase& generator);
    explicit Map(std::vector<std::string> const& map);

    void CarveRoom(Room const& room) override;
    void DigHorizontalTunnel(int x1, int x2, int y) override;
    void DigVerticalTunnel(int y1, int y2, int x) override;
    void DigDownstairs(Point const& point) override;

    Tile const& At(Point const& point) const;
    bool IsOutOfBounds(Point const& point) const;

    ftxui::Element Render(Point const& point) const;

    virtual std::ostream& WriteToStream(std::ostream& os) const;
    friend std::ostream& operator<<(std::ostream& os, Map const& map);

    int m_width;
    int m_height;
    std::vector<Tile> m_tiles;
    std::vector<Room> m_rooms;
};

std::ostream& operator<<(std::ostream& os, Map const& map);

#include "ApprovalTests.hpp"

#include <catch2/catch.hpp>

#include "../src/FovMap.hpp"
#include "../src/Map.hpp"
#include "../src/NaiveMapGenerator.hpp"

#include <random>
#include <string>
#include <vector>

namespace
{

std::vector<std::string> const dungeon{
    "###########################################################",
    "#...........#.............................................#",
    "#...........#........#....................................#",
    "#.....................#...................................#",
    "#....####..............#..................................#",
    "#.......#.......................#####################.....#",
    "#.......#...........................................#.....#",
    "#.......#...........##..............................#.....#",
    "#####........#......##..........##################..#.....#",
    "#...#...........................#................#..#.....#",
    "#...#............#..............#................#..#.....#",
    "#...............................#..###############..#.....#",
    "#...............................#...................#.....#",
    "#...............................#...................#.....#",
    "#...............................#####################.....#",
    "#.........................................................#",
    "#.........................................................#",
    "###########################################################"};

} // namespace

TEST_CASE("MapGenerator")
{
    constexpr int MapWidth = 80;
    constexpr int MapHeight = 45;
    constexpr int RoomMaxSize = 10;
    constexpr int RoomMinSize = 6;
    constexpr int MaxRooms = 30;

    RNG rng(8'109'330);
    NaiveMapGenerator generator(MapWidth, MapHeight, RoomMaxSize, RoomMinSize, MaxRooms, rng);
    Map map(MapWidth, MapHeight, generator);
    ApprovalTests::Approvals::verify(map);
}

TEST_CASE("Map deserialize")
{
    Map map(dungeon);
    ApprovalTests::Approvals::verify(map);
}

TEST_CASE("FOV")
{
    FovMap map(dungeon, 10);
    map.LineOfSight({36, 13});
    ApprovalTests::Approvals::verify(map);
}

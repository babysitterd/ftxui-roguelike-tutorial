#include "ApprovalTests.hpp"

#include <catch2/catch.hpp>

#include "../src/Map.hpp"
#include "../src/MapGenerator.hpp"

#include <random>

TEST_CASE("MapGenerator")
{
    std::random_device::result_type const seed(8'109'330);
    Map map(80, 45, MapGenerator{80, 45, 10, 6, 30, seed});
    ApprovalTests::Approvals::verify(map);
}

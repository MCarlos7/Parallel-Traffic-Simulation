#include "test_framework.hpp"
#include "../include/vehicle/pathfinder.hpp"
#include "../include/city/city.hpp"

using namespace city;
using namespace vehicle;

TEST(PathfinderFindPathSameStartGoal) {
    City::getInstance().initialize(5);
    
    auto path = Pathfinder::findPath(
        Coordinate{2, 2},
        Coordinate{2, 2},
        City::getInstance()
    );
    
    EXPECT_TRUE(path.empty());
}

TEST(PathfinderFindPathAdjacent) {
    City::getInstance().initialize(5);
    
    auto path = Pathfinder::findPath(
        Coordinate{0, 0},
        Coordinate{0, 1},
        City::getInstance()
    );
    
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 2); // Start + goal
}

TEST(PathfinderFindPathLinear) {
    City::getInstance().initialize(5);
    
    auto path = Pathfinder::findPath(
        Coordinate{0, 0},
        Coordinate{0, 4},
        City::getInstance()
    );
    
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.back().x, 0);
    EXPECT_EQ(path.back().y, 4);
}

TEST(PathfinderFindPathDiagonal) {
    City::getInstance().initialize(5);
    
    auto path = Pathfinder::findPath(
        Coordinate{0, 0},
        Coordinate{4, 4},
        City::getInstance()
    );
    
    EXPECT_FALSE(path.empty());
}

TEST(PathfinderFindPathNoObstacles) {
    City::getInstance().initialize(8);
    
    auto path = Pathfinder::findPath(
        Coordinate{1, 1},
        Coordinate{6, 6},
        City::getInstance()
    );
    
    EXPECT_FALSE(path.empty());
    
    // Path length should be Manhattan distance (optimal for grid)
    int expectedLength = 12; // |6-1| + |6-1| = 10 steps, + start = 12 nodes
    EXPECT_EQ(static_cast<int>(path.size()), expectedLength);
}

TEST(PathfinderFindPathCornerToCorner) {
    City::getInstance().initialize(12);
    
    auto path = Pathfinder::findPath(
        Coordinate{0, 0},
        Coordinate{11, 11},
        City::getInstance()
    );
    
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.front().x, 0);
    EXPECT_EQ(path.front().y, 0);
    EXPECT_EQ(path.back().x, 11);
    EXPECT_EQ(path.back().y, 11);
}

// Test OpenMP parallel pathfinding (compile test)
TEST(PathfinderParallelCompilation) {
    City::getInstance().initialize(5);
    
    std::vector<std::tuple<Coordinate, Coordinate, int>> requests;
    requests.push_back({Coordinate{0, 0}, Coordinate{4, 4}, 1});
    requests.push_back({Coordinate{0, 1}, Coordinate{4, 3}, 2});
    
    std::vector<std::pair<int, std::vector<Coordinate>>> results;
    
    // This should compile with OpenMP
    Pathfinder::findPathsParallel(requests, City::getInstance(), results);
    
    EXPECT_EQ(results.size(), 2);
}

RUN_TESTS();

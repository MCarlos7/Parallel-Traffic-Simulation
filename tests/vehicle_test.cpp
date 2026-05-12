#include "test_framework.hpp"
#include "../include/vehicle/vehicle.hpp"
#include "../include/vehicle/pathfinder.hpp"
#include "../include/city/city.hpp"
#include "../include/city/intersection.hpp"

using vehicle::Vehicle;
using vehicle::Pathfinder;
using city::City;
using city::Coordinate;
using traffic_simulation::VehicleState;

TEST(VehicleConstructor) {
    Vehicle v(1);
    EXPECT_EQ(v.getId(), 1);
    EXPECT_EQ(v.getState(), VehicleState::WAITING);
}

TEST(VehicleInitialPosition) {
    Vehicle v(1);
    Coordinate pos = v.getPosition();
    EXPECT_EQ(pos.x, 0);
    EXPECT_EQ(pos.y, 0);
}

TEST(VehicleSetRouteSameStartDest) {
    City::getInstance().initialize(10);
    Vehicle v(1);
    
    bool result = v.setRoute(Coordinate{5, 5}, Coordinate{5, 5}, City::getInstance());
    
    EXPECT_FALSE(result);
    EXPECT_EQ(v.getState(), VehicleState::BLOCKED);
}

TEST(VehicleSetRouteValidPath) {
    City::getInstance().initialize(10);
    Vehicle v(1);
    
    bool result = v.setRoute(Coordinate{0, 0}, Coordinate{1, 0}, City::getInstance());
    
    EXPECT_TRUE(result);
    EXPECT_EQ(v.getState(), VehicleState::WAITING);
}

TEST(VehicleIsArrived) {
    Vehicle v(1);
    EXPECT_TRUE(v.isArrived());
}

TEST(PathfinderHeuristicFromFindPath) {
    City::getInstance().initialize(10);
    
    auto path = Pathfinder::findPath(Coordinate{5, 5}, Coordinate{5, 5}, City::getInstance());
    EXPECT_TRUE(path.empty());
}

TEST(PathfinderBasicPath) {
    City::getInstance().initialize(10);
    
    auto path = Pathfinder::findPath(Coordinate{0, 0}, Coordinate{0, 1}, City::getInstance());
    EXPECT_FALSE(path.empty());
}

TEST(PathfinderLongerPath) {
    City::getInstance().initialize(12);
    
    auto path = Pathfinder::findPath(Coordinate{0, 0}, Coordinate{11, 11}, City::getInstance());
    EXPECT_FALSE(path.empty());
    EXPECT_EQ(path.size(), 23);
}

TEST(PathfinderParallelCompilation) {
    City::getInstance().initialize(5);
    
    std::vector<std::tuple<Coordinate, Coordinate, int>> requests;
    requests.push_back({Coordinate{0, 0}, Coordinate{4, 4}, 1});
    requests.push_back({Coordinate{0, 1}, Coordinate{4, 3}, 2});
    
    std::vector<std::pair<int, std::vector<Coordinate>>> results;
    
    Pathfinder::findPathsParallel(requests, City::getInstance(), results);
    
    EXPECT_EQ(results.size(), 2);
}

RUN_TESTS();

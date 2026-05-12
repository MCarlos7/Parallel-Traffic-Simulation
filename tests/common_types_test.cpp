#include "test_framework.hpp"
#include "../include/city/intersection.hpp"
#include "../include/common/types.hpp"

using namespace city;

TEST(CoordinateEquality) {
    Coordinate a{1, 2};
    Coordinate b{1, 2};
    EXPECT_EQ(a.x, 1);
    EXPECT_EQ(a.y, 2);
    EXPECT_TRUE(a == b);
}

TEST(CoordinateInequality) {
    Coordinate a{1, 2};
    Coordinate b{2, 1};
    EXPECT_TRUE(a != b);
}

TEST(CoordinateAddition) {
    Coordinate a{1, 2};
    Coordinate b{3, 4};
    Coordinate c = a + b;
    EXPECT_EQ(c.x, 4);
    EXPECT_EQ(c.y, 6);
}

TEST(CoordinateSubtraction) {
    Coordinate a{5, 7};
    Coordinate b{2, 3};
    Coordinate c = a - b;
    EXPECT_EQ(c.x, 3);
    EXPECT_EQ(c.y, 4);
}

TEST(CoordinateLessThan) {
    Coordinate a{1, 2};
    Coordinate b{2, 2};
    EXPECT_TRUE(a < b);
    
    Coordinate c{1, 1};
    EXPECT_TRUE(c < a);
}

TEST(CityDirectionEnum) {
    EXPECT_EQ(static_cast<int>(Direction::NORTH), 0);
    EXPECT_EQ(static_cast<int>(Direction::SOUTH), 1);
    EXPECT_EQ(static_cast<int>(Direction::EAST), 2);
    EXPECT_EQ(static_cast<int>(Direction::WEST), 3);
}

TEST(TrafficLightStateEnum) {
    // Using traffic_simulation namespace for these
    EXPECT_EQ(static_cast<int>(traffic_simulation::TrafficLightState::GREEN), 0);
    EXPECT_EQ(static_cast<int>(traffic_simulation::TrafficLightState::YELLOW), 1);
    EXPECT_EQ(static_cast<int>(traffic_simulation::TrafficLightState::RED), 2);
}

TEST(VehicleStateEnum) {
    EXPECT_EQ(static_cast<int>(traffic_simulation::VehicleState::WAITING), 0);
    EXPECT_EQ(static_cast<int>(traffic_simulation::VehicleState::MOVING), 1);
    EXPECT_EQ(static_cast<int>(traffic_simulation::VehicleState::ARRIVED), 2);
    EXPECT_EQ(static_cast<int>(traffic_simulation::VehicleState::BLOCKED), 3);
}

TEST(SimulationConfigDefaults) {
    traffic_simulation::SimulationConfig config;
    EXPECT_EQ(config.grid_size, 12);
    EXPECT_EQ(config.vehicle_count, 10);
    EXPECT_EQ(config.green_duration, 5);
    EXPECT_EQ(config.yellow_duration, 2);
    EXPECT_EQ(config.red_duration, 6);
}

TEST(SimulationConfigCustom) {
    traffic_simulation::SimulationConfig config;
    config.grid_size = 20;
    config.vehicle_count = 50;
    config.green_duration = 10;
    
    EXPECT_EQ(config.grid_size, 20);
    EXPECT_EQ(config.vehicle_count, 50);
    EXPECT_EQ(config.green_duration, 10);
}

RUN_TESTS();

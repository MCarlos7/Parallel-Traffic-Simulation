#include "test_framework.hpp"
#include "../include/city/city.hpp"

using namespace city;

TEST(CitySingleton) {
    City& instance1 = City::getInstance();
    City& instance2 = City::getInstance();
    EXPECT_TRUE(&instance1 == &instance2);
}

TEST(CityInitializeSmall) {
    City::getInstance().initialize(5);
    EXPECT_EQ(City::getInstance().getGridSize(), 5);
    EXPECT_TRUE(City::getInstance().isInitialized());
}

TEST(CityInitializeStandard) {
    City::getInstance().initialize(12);
    EXPECT_EQ(City::getInstance().getGridSize(), 12);
    EXPECT_TRUE(City::getInstance().isInitialized());
}

TEST(IsValidCoordinate) {
    City::getInstance().initialize(10);
    
    EXPECT_TRUE(City::getInstance().isValidCoordinate({0, 0}));
    EXPECT_TRUE(City::getInstance().isValidCoordinate({5, 5}));
    EXPECT_TRUE(City::getInstance().isValidCoordinate({9, 9}));
    
    EXPECT_FALSE(City::getInstance().isValidCoordinate({-1, 0}));
    EXPECT_FALSE(City::getInstance().isValidCoordinate({0, -1}));
    EXPECT_FALSE(City::getInstance().isValidCoordinate({10, 5}));
    EXPECT_FALSE(City::getInstance().isValidCoordinate({5, 10}));
}

TEST(GetIntersection) {
    City::getInstance().initialize(10);
    
    auto intersection = City::getInstance().getIntersection({5, 5});
    ASSERT_TRUE(intersection != nullptr);
    EXPECT_EQ(intersection->getCoordinate().x, 5);
    EXPECT_EQ(intersection->getCoordinate().y, 5);
}

TEST(GetIntersectionAt) {
    City::getInstance().initialize(10);
    
    auto intersection = City::getInstance().getIntersectionAt(3, 7);
    ASSERT_TRUE(intersection != nullptr);
    EXPECT_EQ(intersection->getId(), 3 * 10 + 7);
}

TEST(GetNeighborsCorner) {
    City::getInstance().initialize(10);
    
    auto neighbors = City::getInstance().getNeighbors({0, 0});
    EXPECT_EQ(neighbors.size(), 2); // Only East and South
}

TEST(GetNeighborsCenter) {
    City::getInstance().initialize(10);
    
    auto neighbors = City::getInstance().getNeighbors({5, 5});
    EXPECT_EQ(neighbors.size(), 4); // All four directions
}

TEST(IntersectionLockUnlock) {
    City::getInstance().initialize(10);
    
    auto intersection = City::getInstance().getIntersection({5, 5});
    
    EXPECT_TRUE(intersection->isAvailable());
    
    intersection->lock();
    EXPECT_FALSE(intersection->isAvailable());
    
    intersection->unlock();
    EXPECT_TRUE(intersection->isAvailable());
}

TEST(IntersectionCongestion) {
    City::getInstance().initialize(10);
    
    auto intersection = City::getInstance().getIntersection({5, 5});
    
    EXPECT_EQ(intersection->getCongestion(), 0);
    
    intersection->lock();
    intersection->unlock();
    
    EXPECT_EQ(intersection->getCongestion(), 0);
}

RUN_TESTS();

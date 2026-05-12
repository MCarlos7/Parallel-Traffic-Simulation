#include "test_framework.hpp"
#include "../include/traffic/semaphore.hpp"
#include "../include/traffic/semaphore_controller.hpp"
#include "../include/city/city.hpp"
#include "../include/city/intersection.hpp"
#include <thread>
#include <chrono>

using traffic::Semaphore;
using traffic::SemaphoreController;
using traffic::TrafficLightState;
using city::City;

TEST(SemaphoreConstruction) {
    Semaphore semaphore(1);
    EXPECT_EQ(semaphore.getIntersectionId(), 1);
}

TEST(SemaphoreInitialState) {
    Semaphore semaphore(1);
    EXPECT_FALSE(semaphore.isGreen());
    EXPECT_EQ(semaphore.getState(), TrafficLightState::RED);
}

TEST(SemaphoreStateTransitions) {
    Semaphore semaphore(1);
    
    semaphore.start();
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    semaphore.stop();
}

TEST(SemaphoreControllerInitialize) {
    City::getInstance().initialize(10);
    SemaphoreController controller;
    
    controller.initialize(City::getInstance(), 4);
    
    EXPECT_EQ(controller.getSemaphoreCount(), 4);
    
    controller.stopAll();
}

TEST(SemaphoreControllerGetSemaphore) {
    City::getInstance().initialize(10);
    SemaphoreController controller;
    
    controller.initialize(City::getInstance(), 4);
    
    city::Coordinate coord{5, 5};
    auto semaphore = controller.getSemaphoreAt(coord);
    (void)semaphore;
    
    controller.stopAll();
}

TEST(SemaphoreControllerMultipleInit) {
    City::getInstance().initialize(10);
    SemaphoreController controller;
    
    controller.initialize(City::getInstance(), 4);
    EXPECT_EQ(controller.getSemaphoreCount(), 4);
    
    controller.initialize(City::getInstance(), 2);
    EXPECT_EQ(controller.getSemaphoreCount(), 2);
    
    controller.stopAll();
}

RUN_TESTS();

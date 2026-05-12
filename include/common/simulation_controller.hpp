/**
 * @file simulation_controller.hpp
 * @brief Simulation controller for orchestrating all traffic simulation subsystems
 * @author Traffic Simulation Team
 * @version 1.0
 */

#pragma once

#include "common/types.hpp"
#include "common/logger.hpp"
#include "common/constants.hpp"
#include "city/city.hpp"
#include "traffic/semaphore_controller.hpp"
#include "vehicle/vehicle_manager.hpp"
#include "monitoring/metrics_collector.hpp"

#include <memory>
#include <atomic>
#include <thread>
#include <chrono>

namespace traffic_simulation {

/**
 * @brief Controller that orchestrates all simulation subsystems
 * 
 * The SimulationController manages the City, SemaphoreController,
 * VehicleManager, and MetricsCollector to run a cohesive traffic
 * simulation. It supports both GUI and headless modes.
 */
class SimulationController {
public:
    /**
     * @brief Construct a new SimulationController
     */
    SimulationController();

    ~SimulationController();

    // Non-copyable, non-movable
    SimulationController(const SimulationController&) = delete;
    SimulationController& operator=(const SimulationController&) = delete;
    SimulationController(SimulationController&&) = delete;
    SimulationController& operator=(SimulationController&&) = delete;

    /**
     * @brief Initialize all simulation components
     * @param config Configuration parameters for the simulation
     */
    void initialize(const SimulationConfig& config);

    /**
     * @brief Start the simulation loop
     * @return 0 on normal exit, non-zero on error
     */
    int run();

    /**
     * @brief Pause the simulation
     */
    void pause();

    /**
     * @brief Resume a paused simulation
     */
    void resume();

    /**
     * @brief Set the simulation speed multiplier
     * @param speed Speed multiplier (1.0 = normal, 2.0 = 2x speed, etc.)
     */
    void setSpeed(double speed);

    /**
     * @brief Reset the simulation to initial state
     */
    void reset();

    /**
     * @brief Get reference to the metrics collector
     * @return Reference to MetricsCollector
     */
    city::monitoring::MetricsCollector& getMetrics();

    /**
     * @brief Get reference to the vehicle manager
     * @return Reference to VehicleManager
     */
    vehicle::VehicleManager& getVehicleManager();

    /**
     * @brief Get reference to the semaphore controller
     * @return Reference to SemaphoreController
     */
    traffic::SemaphoreController& getSemaphoreController();

    /**
     * @brief Get reference to the city
     * @return Reference to City
     */
    city::City& getCity();

    /**
     * @brief Check if simulation is running
     * @return true if simulation is running
     */
    bool isRunning() const;

    /**
     * @brief Check if simulation is paused
     * @return true if simulation is paused
     */
    bool isPaused() const;

    /**
     * @brief Get current simulation speed
     * @return Speed multiplier
     */
    double getSpeed() const;

    /**
     * @brief Get current configuration
     * @return Copy of SimulationConfig
     */
    SimulationConfig getConfig() const;

private:
    /**
     * @brief Main simulation tick loop
     */
    void simulationLoop();

    /**
     * @brief Process a single simulation tick
     */
    void processTick();

    city::City city_;
    traffic::SemaphoreController semaphore_controller_;
    vehicle::VehicleManager* vehicle_manager_;
    city::monitoring::MetricsCollector metrics_collector_;

    SimulationConfig config_;
    std::atomic<bool> is_running_;
    std::atomic<bool> is_paused_;
    std::atomic<bool> simulation_complete_;
    std::atomic<double> speed_multiplier_;

    std::thread simulation_thread_;
    std::chrono::steady_clock::time_point last_tick_time_;
};

} // namespace traffic_simulation

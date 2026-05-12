#ifndef MONITORING_METRICS_COLLECTOR_HPP
#define MONITORING_METRICS_COLLECTOR_HPP

#include "metrics.hpp"

#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace city::monitoring {

/**
 * @brief Thread-safe collector for simulation statistics
 * 
 * This class aggregates metrics from multiple vehicles and intersections
 * during simulation runtime in a thread-safe manner.
 */
class MetricsCollector {
public:
    MetricsCollector();
    ~MetricsCollector();
    
    // Disable copy to prevent mutex issues
    MetricsCollector(const MetricsCollector&) = delete;
    MetricsCollector& operator=(const MetricsCollector&) = delete;
    
    // Allow move operations
    MetricsCollector(MetricsCollector&& other) noexcept;
    MetricsCollector& operator=(MetricsCollector&& other) noexcept;
    
    /**
     * @brief Record a vehicle's travel time
     * @param vehicle_id Unique identifier for the vehicle
     * @param time Travel time in seconds
     */
    void recordTravelTime(int vehicle_id, double time);
    
    /**
     * @brief Update congestion level at an intersection
     * @param coord The intersection coordinate
     * @param count Current count of vehicles waiting
     */
    void updateCongestion(const Coordinate& coord, int count);
    
    /**
     * @brief Record a starvation event for a vehicle
     * @param vehicle_id The vehicle that was blocked
     */
    void recordStarvation(int vehicle_id);
    
    /**
     * @brief Record a vehicle arrival at destination
     * @param vehicle_id The arriving vehicle's ID
     */
    void recordArrival(int vehicle_id);
    
    /**
     * @brief Get the average travel time across all vehicles
     * @return Average travel time in seconds, or 0.0 if no vehicles
     */
    double getAverageTravelTime() const;
    
    /**
     * @brief Get the total number of vehicles that have arrived
     * @return Count of arrived vehicles
     */
    std::size_t getTotalVehiclesArrived() const;
    
    /**
     * @brief Get starvation statistics
     * @return Map of vehicle_id -> number of times blocked
     */
    std::map<int, int> getStarvationStatistics() const;
    
    /**
     * @brief Export all metrics to a formatted table
     * @param output_path Path to output file, or empty for stdout
     */
    void exportToTable(const std::string& output_path = "") const;
    
    /**
     * @brief Get vehicle metrics for all arrived vehicles
     * @return Vector of VehicleMetrics sorted by arrival order
     */
    std::vector<VehicleMetrics> getVehicleMetrics() const;
    
    /**
     * @brief Get intersection metrics
     * @return Vector of IntersectionMetrics
     */
    std::vector<IntersectionMetrics> getIntersectionMetrics() const;
    
    /**
     * @brief Get system-wide aggregate metrics
     * @return SystemMetrics struct
     */
    SystemMetrics getSystemMetrics() const;

private:
    std::vector<double> total_travel_times_;
    std::map<Coordinate, std::vector<int>> intersection_congestion_;
    std::map<int, int> starvation_count_;
    std::vector<int> arrival_sequence_;
    mutable std::mutex mutex_;
    
    /**
     * @brief Calculate average from a vector of values
     */
    static double calculateAverage(const std::vector<double>& values);
};

/**
 * @brief Format a double for table output
 * @param value The value to format
 * @param width Minimum character width
 * @return Formatted string
 */
std::string formatDouble(double value, int width);

} // namespace city::monitoring

#endif // MONITORING_METRICS_COLLECTOR_HPP

#ifndef MONITORING_METRICS_HPP
#define MONITORING_METRICS_HPP

#include <cstddef>
#include <vector>
#include <utility>

namespace city::monitoring {

/**
 * @brief Represents a 2D coordinate in the city grid
 */
struct Coordinate {
    int x;
    int y;
    
    Coordinate() : x(0), y(0) {}
    Coordinate(int x, int y) : x(x), y(y) {}
    
    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator<(const Coordinate& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }
};

/**
 * @brief Metrics collected for individual vehicles
 */
struct VehicleMetrics {
    int id;
    double travel_time;
    double wait_time;
    double wait_percentage;
    std::size_t arrival_order;
    
    VehicleMetrics()
        : id(0), travel_time(0.0), wait_time(0.0), wait_percentage(0.0), arrival_order(0) {}
};

/**
 * @brief Metrics for intersection congestion
 */
struct IntersectionMetrics {
    Coordinate coordinate;
    double avg_congestion;
    int max_congestion;
    
    IntersectionMetrics()
        : coordinate(), avg_congestion(0.0), max_congestion(0) {}
};

/**
 * @brief Aggregate system-wide metrics
 */
struct SystemMetrics {
    double avg_travel_time;
    std::size_t total_arrivals;
    std::size_t total_starvations;
    
    SystemMetrics()
        : avg_travel_time(0.0), total_arrivals(0), total_starvations(0) {}
};

} // namespace city::monitoring

#endif // MONITORING_METRICS_HPP

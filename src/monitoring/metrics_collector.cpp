#include "monitoring/metrics_collector.hpp"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cmath>

namespace city::monitoring {

MetricsCollector::MetricsCollector() = default;

MetricsCollector::~MetricsCollector() = default;

MetricsCollector::MetricsCollector(MetricsCollector&& other) noexcept
    : total_travel_times_(std::move(other.total_travel_times_))
    , intersection_congestion_(std::move(other.intersection_congestion_))
    , starvation_count_(std::move(other.starvation_count_))
    , arrival_sequence_(std::move(other.arrival_sequence_))
    , mutex_()
{
}

MetricsCollector& MetricsCollector::operator=(MetricsCollector&& other) noexcept {
    if (this != &other) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::lock_guard<std::mutex> other_lock(other.mutex_);
        total_travel_times_ = std::move(other.total_travel_times_);
        intersection_congestion_ = std::move(other.intersection_congestion_);
        starvation_count_ = std::move(other.starvation_count_);
        arrival_sequence_ = std::move(other.arrival_sequence_);
    }
    return *this;
}

void MetricsCollector::recordTravelTime(int vehicle_id, double time) {
    std::lock_guard<std::mutex> lock(mutex_);
    (void)vehicle_id; // Vehicle ID tracked via arrival_sequence_
    total_travel_times_.push_back(time);
}

void MetricsCollector::updateCongestion(const Coordinate& coord, int count) {
    std::lock_guard<std::mutex> lock(mutex_);
    intersection_congestion_[coord].push_back(count);
}

void MetricsCollector::recordStarvation(int vehicle_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    ++starvation_count_[vehicle_id];
}

void MetricsCollector::recordArrival(int vehicle_id) {
    std::lock_guard<std::mutex> lock(mutex_);
    arrival_sequence_.push_back(vehicle_id);
}

double MetricsCollector::getAverageTravelTime() const {
    std::lock_guard<std::mutex> lock(mutex_);
    if (total_travel_times_.empty()) {
        return 0.0;
    }
    double sum = 0.0;
    for (const double t : total_travel_times_) {
        sum += t;
    }
    return sum / static_cast<double>(total_travel_times_.size());
}

std::size_t MetricsCollector::getTotalVehiclesArrived() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return arrival_sequence_.size();
}

std::map<int, int> MetricsCollector::getStarvationStatistics() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return starvation_count_;
}

std::vector<VehicleMetrics> MetricsCollector::getVehicleMetrics() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<VehicleMetrics> metrics;
    
    const std::size_t arrival_count = arrival_sequence_.size();
    const std::size_t travel_count = total_travel_times_.size();
    
    // Match arrivals with travel times by index
    for (std::size_t i = 0; i < arrival_count; ++i) {
        VehicleMetrics vm;
        vm.id = arrival_sequence_[i];
        vm.arrival_order = i + 1;
        
        // Travel time is recorded before arrival, so index aligns
        if (i < travel_count) {
            vm.travel_time = total_travel_times_[i];
        }
        
        // Estimate wait time from travel time (placeholder - real impl would track directly)
        // For now, leave wait_time at 0.0 - callers can update via record methods
        
        // Calculate wait percentage if we have wait_time data
        if (vm.travel_time > 0.0) {
            // This is a simplified calculation - in production you'd track actual wait time
            vm.wait_percentage = 0.0; // Would be (wait_time / total_time) * 100
        }
        
        metrics.push_back(vm);
    }
    
    return metrics;
}

std::vector<IntersectionMetrics> MetricsCollector::getIntersectionMetrics() const {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<IntersectionMetrics> metrics;
    
    for (const auto& pair : intersection_congestion_) {
        IntersectionMetrics im;
        im.coordinate = pair.first;
        
        const auto& counts = pair.second;
        if (!counts.empty()) {
            double sum = 0.0;
            int max_count = 0;
            for (const int c : counts) {
                sum += c;
                max_count = std::max(max_count, c);
            }
            im.avg_congestion = sum / static_cast<double>(counts.size());
            im.max_congestion = max_count;
        }
        
        metrics.push_back(im);
    }
    
    // Sort by coordinate for consistent output
    std::sort(metrics.begin(), metrics.end(),
        [](const IntersectionMetrics& a, const IntersectionMetrics& b) {
            return a.coordinate < b.coordinate;
        });
    
    return metrics;
}

SystemMetrics MetricsCollector::getSystemMetrics() const {
    std::lock_guard<std::mutex> lock(mutex_);
    
    SystemMetrics sm;
    sm.total_arrivals = arrival_sequence_.size();
    sm.total_starvations = 0;
    
    for (const auto& pair : starvation_count_) {
        sm.total_starvations += static_cast<std::size_t>(pair.second);
    }
    
    if (!total_travel_times_.empty()) {
        double sum = 0.0;
        for (const double t : total_travel_times_) {
            sum += t;
        }
        sm.avg_travel_time = sum / static_cast<double>(total_travel_times_.size());
    }
    
    return sm;
}

double MetricsCollector::calculateAverage(const std::vector<double>& values) {
    if (values.empty()) {
        return 0.0;
    }
    double sum = 0.0;
    for (const double v : values) {
        sum += v;
    }
    return sum / static_cast<double>(values.size());
}

void MetricsCollector::exportToTable(const std::string& output_path) const {
    std::ostringstream output;
    
    const int line_width = 45;
    const std::string separator(line_width, '=');
    const std::string thin_separator(line_width, '-');
    
    // Header
    output << "\n" << separator << "\n";
    output << std::setw(line_width / 2 + 10) << "SIMULATION RESULTS\n";
    output << separator << "\n\n";
    
    // System metrics
    SystemMetrics sys = getSystemMetrics();
    std::vector<VehicleMetrics> vehicles = getVehicleMetrics();
    
    output << "Total Vehicles: " << sys.total_arrivals << "\n";
    output << "Arrived: " << sys.total_arrivals << "\n";
    output << "Average Travel Time: " << std::fixed << std::setprecision(1) 
           << sys.avg_travel_time << "s\n";
    
    // Calculate average wait percentage
    double avg_wait_pct = 0.0;
    if (!vehicles.empty()) {
        double sum = 0.0;
        for (const auto& v : vehicles) {
            sum += v.wait_percentage;
        }
        avg_wait_pct = sum / static_cast<double>(vehicles.size());
    }
    output << "Average Wait Percentage: " << std::fixed << std::setprecision(1) 
           << avg_wait_pct << "%\n";
    
    // Vehicle arrival order
    output << "\n" << thin_separator << "\n";
    output << "VEHICLE ARRIVAL ORDER:\n";
    output << thin_separator << "\n";
    
    if (!vehicles.empty()) {
        // Column headers
        output << std::setw(6) << "ID" << " | ";
        output << std::setw(12) << "Travel Time" << " | ";
        output << std::setw(8) << "Wait %" << " | ";
        output << std::setw(8) << "Arrival" << "\n";
        output << thin_separator << "\n";
        
        for (const auto& v : vehicles) {
            output << std::setw(6) << v.id << " | ";
            output << std::setw(10) << std::fixed << std::setprecision(1) 
                   << v.travel_time << "s | ";
            output << std::setw(6) << std::fixed << std::setprecision(1) 
                   << v.wait_percentage << "% | ";
            output << std::setw(8) << v.arrival_order << "\n";
        }
    } else {
        output << "No vehicles recorded.\n";
    }
    
    // Intersection congestion
    std::vector<IntersectionMetrics> intersections = getIntersectionMetrics();
    output << "\n" << thin_separator << "\n";
    output << "INTERSECTION CONGESTION:\n";
    output << thin_separator << "\n";
    
    if (!intersections.empty()) {
        output << std::setw(8) << "X,Y" << " | ";
        output << std::setw(10) << "Avg Wait" << " | ";
        output << std::setw(10) << "Max Wait" << "\n";
        output << thin_separator << "\n";
        
        for (const auto& im : intersections) {
            output << std::setw(3) << im.coordinate.x << "," 
                   << std::setw(3) << im.coordinate.y << " | ";
            output << std::setw(8) << std::fixed << std::setprecision(1) 
                   << im.avg_congestion << " | ";
            output << std::setw(8) << im.max_congestion << "\n";
        }
    } else {
        output << "No intersection data recorded.\n";
    }
    
    output << "\n" << separator << "\n";
    
    // Output to file or stdout
    if (!output_path.empty()) {
        std::ofstream file(output_path);
        if (file.is_open()) {
            file << output.str();
            file.close();
        } else {
            std::cerr << "Warning: Could not open file " << output_path 
                      << ", outputting to stdout instead.\n";
            std::cout << output.str();
        }
    } else {
        std::cout << output.str();
    }
}

} // namespace city::monitoring

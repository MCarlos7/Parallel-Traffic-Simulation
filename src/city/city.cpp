#include "city/city.hpp"
#include "city/intersection.hpp"
#include "city/street.hpp"
#include <stdexcept>
#include <algorithm>

namespace city {

void City::initialize(int grid_size) {
    if (grid_size <= 0) {
        throw std::invalid_argument("Grid size must be positive");
    }

    grid_size_ = grid_size;
    intersections_.clear();
    intersections_.resize(grid_size);

    initializeIntersections();
}

void City::initializeIntersections() {
    size_t id = 0;
    for (int y = 0; y < grid_size_; ++y) {
        intersections_[y].reserve(grid_size_);
        for (int x = 0; x < grid_size_; ++x) {
            auto intersection = std::make_shared<Intersection>(id++, x, y);
            intersections_[y].push_back(intersection);
        }
    }
}

std::shared_ptr<Intersection> City::getIntersection(const Coordinate& coord) const {
    return getIntersectionAt(coord.x, coord.y);
}

std::shared_ptr<Intersection> City::getIntersectionAt(int x, int y) const {
    if (!isValidCoordinate({x, y})) {
        return nullptr;
    }
    return intersections_[y][x];
}

std::vector<std::shared_ptr<Intersection>> City::getNeighbors(const Coordinate& coord) const {
    std::vector<std::shared_ptr<Intersection>> neighbors;

    // Four cardinal directions: NORTH, SOUTH, EAST, WEST
    // x+1 = EAST, x-1 = WEST, y-1 = NORTH, y+1 = SOUTH
    const std::vector<std::pair<int, int>> directions = {
        {0, -1},  // NORTH
        {0, 1},   // SOUTH
        {1, 0},   // EAST
        {-1, 0}   // WEST
    };

    for (const auto& [dx, dy] : directions) {
        auto neighbor_coord = Coordinate{coord.x + dx, coord.y + dy};
        if (isValidCoordinate(neighbor_coord)) {
            if (auto neighbor = getIntersection(neighbor_coord)) {
                neighbors.push_back(neighbor);
            }
        }
    }

    return neighbors;
}

bool City::isValidCoordinate(const Coordinate& coord) const {
    return coord.x >= 0 && coord.x < grid_size_ &&
           coord.y >= 0 && coord.y < grid_size_;
}

void City::addStreet(std::shared_ptr<StreetSegment> street) {
    if (!street) {
        throw std::invalid_argument("Street cannot be null");
    }
    streets_.push_back(std::move(street));
}

const std::vector<std::shared_ptr<StreetSegment>>& City::getStreets() const {
    return streets_;
}

int City::getGridSize() const {
    return grid_size_;
}

bool City::isInitialized() const {
    return grid_size_ > 0 && !intersections_.empty() &&
           intersections_.size() == static_cast<size_t>(grid_size_);
}

}  // namespace city

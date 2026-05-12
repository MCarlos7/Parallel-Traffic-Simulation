#include "city/intersection.hpp"
#include <stdexcept>

namespace city {

Intersection::Intersection(size_t id, int x, int y)
    : id_(id), x_(x), y_(y), is_occupied_(false), congestion_level_(0) {}

void Intersection::lock() {
    mutex_.lock();
    is_occupied_ = true;
}

bool Intersection::try_lock() {
    if (mutex_.try_lock()) {
        is_occupied_ = true;
        return true;
    }
    return false;
}

void Intersection::unlock() {
    is_occupied_ = false;
    mutex_.unlock();
}

bool Intersection::isAvailable() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return !is_occupied_;
}

int Intersection::getCongestion() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return congestion_level_;
}

size_t Intersection::getId() const {
    return id_;
}

Coordinate Intersection::getCoordinate() const {
    return {x_, y_};
}

}  // namespace city

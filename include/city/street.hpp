#pragma once

#include "intersection.hpp"
#include <memory>

namespace city {

class Intersection;

class StreetSegment {
public:
    StreetSegment() = default;
    StreetSegment(std::shared_ptr<Intersection> start,
                  std::shared_ptr<Intersection> end,
                  bool is_two_way = true);

    // Non-copyable
    StreetSegment(const StreetSegment&) = delete;
    StreetSegment& operator=(const StreetSegment&) = delete;

    // Accessors
    std::shared_ptr<Intersection> getStartIntersection() const;
    std::shared_ptr<Intersection> getEndIntersection() const;
    bool isTwoWay() const;
    Direction getDirection() const;

    // Travel query
    bool canTravel(Direction direction) const;
    Direction getOppositeDirection() const;

private:
    std::shared_ptr<Intersection> start_intersection_;
    std::shared_ptr<Intersection> end_intersection_;
    bool is_two_way_;
    Direction direction_;
};

}  // namespace city

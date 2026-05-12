#include "monitoring/metrics.hpp"

#include <iomanip>
#include <sstream>
#include <cmath>

namespace city::monitoring {

std::string formatDouble(double value, int width) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << value;
    std::string result = oss.str();
    if (static_cast<int>(result.length()) < width) {
        result = std::string(width - result.length(), ' ') + result;
    }
    return result;
}

} // namespace city::monitoring

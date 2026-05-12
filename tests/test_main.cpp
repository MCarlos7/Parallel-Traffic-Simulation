#include "test_framework.hpp"
#include "../include/city/city.hpp"

int main(int argc, char** argv) {
    // Initialize city with default grid size for tests
    city::City::getInstance().initialize(12);
    
    std::cout << "========================================\n";
    std::cout << "  Traffic Simulation - Unit Tests\n";
    std::cout << "========================================\n\n";
    
    return TestRunner::getInstance().runTests();
}

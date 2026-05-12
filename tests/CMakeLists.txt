cmake_minimum_required(VERSION 3.10)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Find Threads
find_package(Threads REQUIRED)

# Include directories
set(INCLUDE_DIRS
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/city
    ${CMAKE_SOURCE_DIR}/include/traffic
    ${CMAKE_SOURCE_DIR}/include/vehicle
    ${CMAKE_SOURCE_DIR}/include/monitoring
    ${CMAKE_SOURCE_DIR}/include/gui
    ${CMAKE_SOURCE_DIR}/include/common
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Test executable
add_executable(run_tests
    test_main.cpp
    common_types_test.cpp
    city_test.cpp
    traffic_test.cpp
    vehicle_test.cpp
    pathfinder_test.cpp
)

target_include_directories(run_tests PRIVATE ${INCLUDE_DIRS})

target_link_libraries(run_tests PRIVATE
    Threads
    OpenMP::OpenMP_CXX
)

# Enable C++17
target_compile_features(run_tests PRIVATE cxx_std_17)

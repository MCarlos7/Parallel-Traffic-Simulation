cmake_minimum_required(VERSION 3.10)
project(traffic_simulation VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler flags
add_compile_options(-Wall -Wextra)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Find SFML with components
find_package(SFML 2.5 COMPONENTS graphics window system QUIET)

# If SFML not found via find_package, try pkg-config
if(NOT SFML_FOUND)
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(SFML REQUIRED sfml-graphics sfml-window sfml-system)
    endif()
endif()

# Add source files from all src/* directories
file(GLOB_RECURSE SOURCES 
    "src/*/*.cpp"
)

# Include directories
set(INCLUDE_DIRS
    include
    include/city
    include/traffic
    include/vehicle
    include/monitoring
    include/gui
    include/common
)

# Create executable
add_executable(traffic_simulation ${SOURCES})

# Include directories
target_include_directories(traffic_simulation PRIVATE ${INCLUDE_DIRS})

# Link libraries
if(SFML_FOUND)
    target_link_libraries(traffic_simulation PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
    )
elseif(SFML_LIBRARIES)
    target_link_libraries(traffic_simulation PRIVATE
        ${SFML_LIBRARIES}
    )
endif()

# Thread and OpenMP
target_link_libraries(traffic_simulation PRIVATE
    pthread
    OpenMP::OpenMP_CXX
)

# Default to Release build
if(CMAKE_BUILD_TYPE STREQUAL "")
    set(CMAKE_BUILD_TYPE Release)
endif()

# Install target
install(TARGETS traffic_simulation DESTINATION bin)

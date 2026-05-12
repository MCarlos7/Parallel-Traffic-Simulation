# Traffic Simulation

A high-performance traffic simulation system demonstrating concurrency patterns, pathfinding algorithms, and parallel computing. Built with C++17, SFML for visualization, and OpenMP for parallel processing.

## Features

- **Concurrent Traffic Management**: Thread-safe vehicle movement and traffic light state management using mutexes and condition variables
- **A\* Pathfinding**: Efficient shortest-path computation for vehicle routing through the city grid
- **OpenMP Parallelization**: Parallel processing for vehicle updates and traffic light cycling
- **Interactive GUI**: Real-time visualization with SFML showing vehicles, traffic lights, and road network

## Requirements

- C++17 or later
- CMake 3.10+
- [SFML](https://www.sfml-dev.org/) 2.5+
- OpenMP

### Installing Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get install cmake libsfml-dev libomp-dev
```

**macOS (Homebrew):**
```bash
brew install cmake sfml libomp
```

**Windows:**
Install via [vcpkg](https://github.com/microsoft/vcpkg) or download from [SFML website](https://www.sfml-dev.org/download.php).

## Building

```bash
mkdir build && cd build
cmake ..
make
```

The compiled executable `traffic_simulation` will be placed in the `build` directory.

## Usage

```bash
./traffic_simulation [options]
```

### Command Line Options

| Option | Description | Default |
|--------|-------------|---------|
| `--no-gui` | Run simulation without GUI (headless mode) | `false` |
| `--vehicles <N>` | Number of vehicles in simulation | `10` |
| `--grid <N>` | Grid size (N x N intersections) | `5` |
| `--green <N>` | Green light duration (seconds) | `5` |
| `--yellow <N>` | Yellow light duration (seconds) | `2` |
| `--red <N>` | Red light duration (seconds) | `5` |

### Examples

Run with default settings:
```bash
./traffic_simulation
```

Run headless with 20 vehicles on a 7x7 grid:
```bash
./traffic_simulation --no-gui --vehicles 20 --grid 7
```

Custom traffic light timings:
```bash
./traffic_simulation --green 8 --yellow 3 --red 10
```

## Project Structure

```
concuproyect/
├── CMakeLists.txt          # Build configuration
├── README.md                # This file
├── .gitignore               # Git ignore rules
├── docs/
│   └── scheme.drawio        # Architecture diagram
├── include/
│   └── concurrency-project/
│       ├── Intersection.hpp
│       ├── TrafficLight.hpp
│       ├── Vehicle.hpp
│       ├── CityGrid.hpp
│       ├── AStar.hpp
│       └── Simulation.hpp
├── src/
│   ├── main.cpp             # Application entry point
│   ├── Intersection.cpp
│   ├── TrafficLight.cpp
│   ├── Vehicle.cpp
│   ├── CityGrid.cpp
│   ├── AStar.cpp
│   └── Simulation.cpp
└── tests/
    └── test_main.cpp
```

## Architecture

The simulation follows a concurrent producer-consumer pattern where:

1. **CityGrid** manages the road network and intersections
2. **TrafficLight** objects coordinate vehicle flow at intersections using condition variables
3. **Vehicle** objects navigate using A* pathfinding
4. **Simulation** orchestrates the parallel execution with OpenMP

For a detailed architecture diagram, see [docs/scheme.drawio](docs/scheme.drawio).

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

Please ensure code follows existing style and includes appropriate tests.

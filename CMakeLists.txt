cmake_minimum_required(VERSION 3.10)
project(traffic_simulation VERSION 1.0 LANGUAGES CXX)

# Configuraciones de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra)

# Buscar OpenMP
find_package(OpenMP REQUIRED)

# Agregar todos los archivos fuente
file(GLOB_RECURSE SOURCES "src/*.cpp")

# If SFML not found via find_package, try pkg-config
if(NOT SFML_FOUND)
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(SFML REQUIRED sfml-graphics sfml-window sfml-system)
    endif()
endif()

# Add source files from all src/* directories
file(GLOB_RECURSE SOURCES 
    "src/*.cpp" "src/*/*.cpp"
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

# Crear el ejecutable
add_executable(traffic_simulation ${SOURCES})
target_include_directories(traffic_simulation PRIVATE ${INCLUDE_DIRS})

# Enlace DIRECTO de las librerías (sin intermediarios que inyecten -mwindows)
target_link_libraries(traffic_simulation PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    pthread
    OpenMP::OpenMP_CXX
)

# EL GOLPE DE GRACIA: Forzar a Windows a usar la consola
target_link_options(traffic_simulation PRIVATE "-mconsole")
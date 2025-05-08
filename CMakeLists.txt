cmake_minimum_required(VERSION 3.20)
project(CppFrogger VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add executable
add_executable(frogger 
    src/main.cpp
    src/Game.cpp
    src/Player.cpp
    src/Vehicle.cpp
    src/Log.cpp
    src/Crocodile.cpp
    src/GameEntity.cpp
)

# Link SFML libraries
target_link_libraries(frogger PRIVATE sfml-graphics sfml-window sfml-system)

# Include directories
target_include_directories(frogger PRIVATE include)

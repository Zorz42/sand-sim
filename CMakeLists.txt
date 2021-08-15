cmake_minimum_required(VERSION 3.20)
project(sand_sim)

set(CMAKE_CXX_STANDARD 14)

add_executable(sand_sim
        "SandSimulation/main.cpp" "SandSimulation/materialClass/materialClass.hpp" "SandSimulation/materialClass/materialClass.cpp" SandSimulation/particleContainer/particleContainer.cpp SandSimulation/particleContainer/particleContainer.hpp SandSimulation/renderer/renderer.cpp)

target_include_directories(sand_sim PUBLIC SandSimulation/materialClass SandSimulation/particleContainer SandSimulation/renderer)

target_link_libraries(sand_sim sfml-system sfml-graphics sfml-window)
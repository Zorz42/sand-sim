cmake_minimum_required(VERSION 3.20)
project(sand_sim)

set(CMAKE_CXX_STANDARD 14)

add_executable(sand_sim
        "SandSimulation/main.cpp" "SandSimulation/updateFunctions.cpp" "SandSimulation/updateFunctions.hpp" SandSimulation/materialClass.hpp)

target_link_libraries(sand_sim SDL2)
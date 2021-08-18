cmake_minimum_required(VERSION 3.20)
project(sand_sim)

set(CMAKE_CXX_STANDARD 17)

add_executable(sand_sim
        SandSimulation/main.cpp SandSimulation/particles/particles.cpp SandSimulation/particleEngine/particleEngine.cpp SandSimulation/particleRenderer/particleRenderer.cpp)

target_include_directories(sand_sim PUBLIC SandSimulation/materialClass SandSimulation/particleEngine SandSimulation/particleRenderer SandSimulation/particles)

target_link_libraries(sand_sim sfml-system sfml-graphics sfml-window)
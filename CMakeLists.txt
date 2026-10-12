cmake_minimum_required(VERSION 3.11)

project(fluid_simulation C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAME_MODES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/raylib-6.0)

add_executable(fluid_simulation src/main.c)

target_link_libraries(fluid_simulation PRIVATE raylib)

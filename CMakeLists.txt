cmake_minimum_required(VERSION 4.0)

project(HeroDungeonSim)

# find_package(nlohmann_json 3.11.3 REQUIRED)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(json)

add_executable(Game main.cpp)

target_link_libraries(Game PRIVATE nlohmann_json::nlohmann_json)

# find_package(nlohmann_json 3.11.3 REQUIRED)

add_subdirectory(json)

add_executable(Game main.cpp)

target_link_libraries(Game PRIVATE nlohmann_json::nlohmann_json)

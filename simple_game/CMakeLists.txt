cmake_minimum_required(VERSION 3.0)
project(simple-game)
set (CMAKE_CXX_STANDARD 14)
set(SOURCES
    src/simple_game.cpp
)
set(HEADERS
)
add_library(simple-game SHARED ${SOURCES} ${HEADERS})
target_include_directories(simple-game PUBLIC include)
target_link_libraries(simple-game sigma-core)

add_resource_package(simple-game-resources resources)
add_dependencies(simple-game simple-game-resources)

cmake_minimum_required(VERSION 3.0)
project(simple-game)

set (CMAKE_CXX_STANDARD 14)

set(SOURCES
    src/simple_game.cpp
)

set(HEADERS
    include/simple_game_config.hpp
)

add_library(simple-game SHARED ${SOURCES} ${HEADERS})
target_compile_definitions(simple-game PUBLIC -DSIMPLE_GAME_SHARED PRIVATE -DSIMPLE_GAME_EXPORT)
target_include_directories(simple-game PUBLIC include resources ../resources)
target_link_libraries(simple-game sigma-core)

file(GLOB_RECURSE RESOURCES resources resources/*.*)
add_resources(simple-game PACKAGE_ROOT resources ${RESOURCES})

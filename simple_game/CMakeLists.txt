set(SOURCES
    src/main.cpp
    src/simple_game.cpp
)

set(HEADERS
    include/grid_component.hpp
    include/simple_game.hpp
)

add_executable(simple-game ${SOURCES} ${HEADERS})
target_include_directories(simple-game PUBLIC include resources ../resources)
target_link_libraries(simple-game sigma-core)

file(GLOB_RECURSE RESOURCES resources resources/*.*)
add_resources(simple-game PACKAGE_ROOT resources ${RESOURCES})

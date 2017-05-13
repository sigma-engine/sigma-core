set(SOURCES
    src/main.cpp
    src/simple_game.cpp
)

set(HEADERS
    include/grid_component.hpp
    include/simple_game.hpp
)

generate_meta_data(GENERATED_SOURCES ${HEADERS})
add_executable(simple-game ${SOURCES} ${GENERATED_SOURCES})
target_include_directories(simple-game PUBLIC include ${CMAKE_CURRENT_BINARY_DIR}/include resources ../resources)
target_link_libraries(simple-game sigma-core)

file(GLOB_RECURSE RESOURCES resources resources/*.*)
add_resources(simple-game PACKAGE_ROOT resources ${RESOURCES})

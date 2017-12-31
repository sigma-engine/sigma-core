add_executable(simple-game
    src/main.cpp
    src/simple_game.cpp
)
target_include_directories(simple-game
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(simple-game
    PUBLIC
    sigma-core
    sigma-opengl-graphics
    sigma-tools
)

configure_file(settings.json ${CMAKE_BINARY_DIR}/data/settings.json)

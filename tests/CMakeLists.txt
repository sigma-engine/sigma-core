set(SOURCES
    main.cpp
    world_tests.cpp
)

set(HEADERS
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigma-core-tests ${HEADERS} ${SOURCES})
target_link_libraries(sigma-core-tests sigma-core)

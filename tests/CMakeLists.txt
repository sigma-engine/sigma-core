set(SOURCES
    main.cpp
    entity_manager_tests.cpp
)

set(HEADERS
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigma-five-engine-tests ${HEADERS} ${SOURCES})
target_link_libraries(sigma-five-engine-tests gtest gtest_main gmock gmock_main sigma-five-engine)

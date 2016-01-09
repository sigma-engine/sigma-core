add_package(googletest)

set(SOURCES
    main.cpp
    resource/resource_tests.cpp
)

set(HEADERS
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigma-five-engine-tests ${HEADERS} ${SOURCES})
target_link_libraries(sigma-five-engine-tests gmock gmock_main sigma-five-engine)

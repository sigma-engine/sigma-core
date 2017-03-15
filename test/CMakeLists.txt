find_package(GTest REQUIRED)

set(TEST_SOURCES
    sigma/main.cpp
    sigma/world_tests.cpp
)

set(TEST_HEADERS
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigma-core-tests ${TEST_HEADERS} ${TEST_SOURCES})
target_link_libraries(sigma-core-tests sigma-core ${CONAN_LIBS})
gtest_add_tests(sigma-core-tests "" AUTO)

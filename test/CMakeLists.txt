enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigma-core-tests
    sigma/main.cpp
    sigma/world_tests.cpp
    sigma/frustum_tests.cpp
)
target_link_libraries(sigma-core-tests
    PRIVATE
    sigma-core
    GTest::GTest
    GTest::Main
    gmock
)

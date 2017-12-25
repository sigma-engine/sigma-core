enable_testing()

find_package(GTest REQUIRED)
#find_package(GMock REQUIRED)

set(SOURCES
    sigma/main.cpp
    sigma/world_tests.cpp
    sigma/frustum_tests.cpp
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigma-core-tests ${SOURCES})
target_link_libraries(sigma-core-tests PRIVATE Boost::boost glm GTest::GTest GTest::Main sigma-core gmock)

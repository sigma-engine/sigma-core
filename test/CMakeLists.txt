find_package(GTest REQUIRED)

set(SOURCES
    sigma/main.cpp
    sigma/world_tests.cpp
    sigma/frustum_tests.cpp
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigma-core-tests ${SOURCES})
target_link_libraries(sigma-core-tests sigma-core CONAN_PKG::gtest)
gtest_add_tests(sigma-core-tests "" AUTO)

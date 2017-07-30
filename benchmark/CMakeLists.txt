set(SOURCES
    sigma/main.cpp
    sigma/world_benchmarks.cpp
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigma-core-benchmarks ${SOURCES})
target_link_libraries(sigma-core-benchmarks sigma-core ${CONAN_LIBS})

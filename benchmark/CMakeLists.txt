set(BENCHMARK_SOURCES
    sigma/main.cpp
    sigma/world_benchmarks.cpp
)

set(BENCHMARK_HEADERS
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_executable(sigma-core-benchmarks ${BENCHMARK_HEADERS} ${BENCHMARK_SOURCES})
target_link_libraries(sigma-core-benchmarks sigma-core ${CONAN_LIBS})

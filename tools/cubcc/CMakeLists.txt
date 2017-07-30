set(SOURCES
    src/main.cpp
)

add_executable(cubcc ${SOURCES})
target_include_directories(cubcc PUBLIC include)
target_link_libraries(cubcc sigma-core)

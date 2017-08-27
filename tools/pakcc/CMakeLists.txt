set(SOURCES
    src/main.cpp
)

add_executable(pakcc ${SOURCES})
target_include_directories(pakcc PUBLIC include)
target_link_libraries(pakcc sigma-core)

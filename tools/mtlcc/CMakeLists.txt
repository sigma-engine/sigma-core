set(SOURCES
    src/main.cpp
)

add_executable(mtlcc ${SOURCES})
target_include_directories(mtlcc PUBLIC include)
target_link_libraries(mtlcc sigma-core)

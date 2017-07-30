set(SOURCES
    src/main.cpp
)
set(HEADERS
)

add_executable(mtlcc ${SOURCES} ${HEADERS})
target_include_directories(mtlcc PUBLIC include)
target_link_libraries(mtlcc sigma-core)

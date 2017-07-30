set(SOURCES
    src/main.cpp
)
set(HEADERS
)

add_executable(cubcc ${SOURCES} ${HEADERS})
target_include_directories(cubcc PUBLIC include)
target_link_libraries(cubcc sigma-core)

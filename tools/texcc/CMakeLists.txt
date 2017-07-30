set(SOURCES
    src/main.cpp
)

add_executable(texcc ${SOURCES})
target_include_directories(texcc PUBLIC include)
target_link_libraries(texcc sigma-core)

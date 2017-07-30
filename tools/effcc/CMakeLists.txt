set(SOURCES
    src/main.cpp
)
set(HEADERS
)

add_executable(effcc ${SOURCES} ${HEADERS})
target_include_directories(effcc PUBLIC include)
target_link_libraries(effcc sigma-core)

set(SOURCES
    src/main.cpp
)
set(HEADERS
)

add_executable(shdcc ${SOURCES} ${HEADERS})
target_include_directories(shdcc PUBLIC include)
target_link_libraries(shdcc sigma-core)

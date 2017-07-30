set(SOURCES
    src/main.cpp
)

add_executable(shdcc ${SOURCES})
target_include_directories(shdcc PUBLIC include)
target_link_libraries(shdcc sigma-core)

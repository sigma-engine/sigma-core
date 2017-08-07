set(SOURCES
    src/main.cpp
)

add_executable(mdlcc ${SOURCES})
target_include_directories(mdlcc PUBLIC include)
target_link_libraries(mdlcc sigma-core)

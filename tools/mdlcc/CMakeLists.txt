set(SOURCES
    src/main.cpp
    src/assimp_converter.cpp
)

add_executable(mdlcc ${SOURCES})
target_include_directories(mdlcc PUBLIC include)
target_link_libraries(mdlcc sigma-core)

set(SOURCES
    src/main.cpp
    src/assimp_converter.cpp
)
set(HEADERS
    include/assimp_converter.hpp
)

add_executable(mdlcc ${SOURCES} ${HEADERS})
target_include_directories(mdlcc PUBLIC include)
target_link_libraries(mdlcc sigma-core)

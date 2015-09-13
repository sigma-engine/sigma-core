project(assimp_import_plugin)

find_package(assimp REQUIRED)

set(SOURCES src/assimp_import_plugin.cpp)
set(HEADERS include/assimp_import_plugin.hpp)

link_directories(${ASSIMP_LIBRARY_DIRS})
add_plugin(assimp_import_plugin ${SOURCES} ${HEADERS})
target_include_directories(assimp_import_plugin PUBLIC include)
target_link_libraries(assimp_import_plugin editor-core ${ASSIMP_LIBRARIES})
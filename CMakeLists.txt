cmake_minimum_required(VERSION 3.1)
project(sigma-engine)

include(${CMAKE_SOURCE_DIR}/sigma_setup.cmake)
if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

set (CMAKE_CXX_STANDARD 14)
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES ".*[cC][lL][aA][nN][gG]")
    set(CMAKE_CXX_FLAGS "-fPIC")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

set(LIBRARY_SOURCES
    src/sigma/game.cpp
    src/sigma/entity.cpp
    src/sigma/context.cpp
    src/sigma/entity_manager.cpp
    src/sigma/util/filesystem.cpp
    src/sigma/graphics/shader.cpp
    src/sigma/graphics/renderer.cpp
    src/sigma/graphics/texture.cpp
    src/sigma/graphics/material.cpp
    src/sigma/resource/identifier.cpp
    src/sigma/graphics/static_mesh.cpp
    src/sigma/graphics/static_mesh_instance.cpp
)
set(LIBRARY_HEADERS
    include/sigma/game.hpp
    include/sigma/config.hpp
    include/sigma/entity.hpp
    include/sigma/context.hpp
    include/sigma/entity_manager.hpp
    include/sigma/component.hpp
    include/sigma/transform.hpp
    include/sigma/util/filesystem.hpp
    include/sigma/graphics/shader.hpp
    include/sigma/graphics/renderer.hpp
    include/sigma/graphics/texture.hpp
    include/sigma/reflect/reflect.hpp
    include/sigma/graphics/material.hpp
    include/sigma/graphics/spot_light.hpp
    include/sigma/resource/identifier.hpp
    include/sigma/graphics/point_light.hpp
    include/sigma/graphics/static_mesh.hpp
    include/sigma/util/glm_serialize.hpp
    include/sigma/graphics/directional_light.hpp
    include/sigma/util/compile_time_hash.hpp
    include/sigma/graphics/static_mesh_instance.hpp
    include/sigma/resource/basic_resource_cache.hpp
)

add_library(sigma-core STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(sigma-core PUBLIC include)
target_link_libraries(sigma-core PUBLIC ${CMAKE_DL_LIBS} ${CONAN_LIBS})
#target_compile_definitions(sigma-core PUBLIC -DSIGMA_SHARED)
#target_compile_options(sigma-core PRIVATE -DSIGMA_EXPORT)

set(MAIN_SOURCES
    src/sigma/main.cpp
)
set(MAIN_HEADERS
)

add_executable(sigma-runner ${MAIN_SOURCES} ${MAIN_HEADERS})
target_link_libraries(sigma-runner PUBLIC sigma-core)

add_subdirectory(tools/scmaterial)
add_subdirectory(tools/scmodel)
add_subdirectory(tools/screflect)
add_subdirectory(tools/scshader)
add_subdirectory(tools/sctexture)
add_subdirectory(tools/seditor)
add_subdirectory(backends/graphics/sopengl)


#add_subdirectory(tests)

add_subdirectory(simple_game)

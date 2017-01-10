cmake_minimum_required(VERSION 3.7)
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

if (MSVC)
    # TODO fix these warnings in visual studio
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4275 /wd4251 /wd4996")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_BINARY_DIR}/lib)
endforeach(OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES)

set(LIBRARY_SOURCES
    src/sigma/context.cpp
    src/sigma/entity_manager.cpp
    src/sigma/entity.cpp
    src/sigma/game.cpp
    src/sigma/trackball_controller.cpp
    src/sigma/transform.cpp

    src/sigma/graphics/renderer.cpp
    src/sigma/graphics/static_mesh_instance.cpp
    src/sigma/resource/identifier.cpp
    src/sigma/graphics/material.cpp
    src/sigma/graphics/post_process_effect.cpp
    src/sigma/graphics/static_mesh.cpp

    src/sigma/util/filesystem.cpp
    src/sigma/util/json_conversion.cpp
)
set(LIBRARY_HEADERS
    include/sigma/component.hpp
    include/sigma/config.hpp
    include/sigma/context.hpp
    include/sigma/entity_manager.hpp
    include/sigma/entity.hpp
    include/sigma/game.hpp
    include/sigma/trackball_controller.hpp
    include/sigma/transform.hpp

    include/sigma/graphics/cubemap.hpp
    include/sigma/graphics/directional_light.hpp
    include/sigma/graphics/material.hpp
    include/sigma/graphics/point_light.hpp
    include/sigma/graphics/post_process_effect.hpp
    include/sigma/graphics/renderer.hpp
    include/sigma/graphics/scene.hpp
    include/sigma/graphics/shader.hpp
    include/sigma/graphics/static_mesh.hpp
    include/sigma/graphics/spot_light.hpp
    include/sigma/graphics/static_mesh_instance.hpp
    include/sigma/graphics/texture.hpp

    include/sigma/reflect/reflect.hpp

    include/sigma/resource/identifier.hpp
    include/sigma/resource/manager.hpp

    include/sigma/util/compile_time_hash.hpp
    include/sigma/util/filesystem.hpp
    include/sigma/util/glm_serialize.hpp
    include/sigma/util/json_conversion.hpp
)

add_library(sigma-core SHARED ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})
target_include_directories(sigma-core PUBLIC include)
target_link_libraries(sigma-core PUBLIC ${CMAKE_DL_LIBS} ${CONAN_LIBS})
target_compile_definitions(sigma-core PUBLIC -DSIGMA_SHARED)
target_compile_options(sigma-core PRIVATE -DSIGMA_EXPORT)

add_subdirectory(tools/scmaterial)
add_subdirectory(tools/scmodel)
add_subdirectory(tools/screflect)
add_subdirectory(tools/scshader)
add_subdirectory(tools/sctexture)
add_subdirectory(tools/seditor)
add_subdirectory(backends/graphics/sopengl)

## add_subdirectory(tests)

add_subdirectory(simple_game)

set(MAIN_SOURCES
    src/sigma/main.cpp
    src/sigma/window.cpp
)
set(MAIN_HEADERS
    include/sigma/window.hpp
)

add_executable(sigma-runner ${MAIN_SOURCES} ${MAIN_HEADERS})
target_link_libraries(sigma-runner PUBLIC sigma-core)
add_dependencies(sigma-runner simple-game sopengl)

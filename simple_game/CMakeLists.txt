add_executable(simple-game
    src/main.cpp
    src/simple_game.cpp
)
target_include_directories(simple-game
    PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_link_libraries(simple-game
    PUBLIC
    sigma-core
    sigma-opengl-graphics
    sigma-tools
)

configure_file(settings.json ${CMAKE_BINARY_DIR}/data/settings.json)

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "../resources/*.vert"
    "../resources/*.tesc"
    "../resources/*.tese"
    "../resources/*.geom"
    "../resources/*.frag"
)
foreach(GLSL ${GLSL_SOURCE_FILES})
    file(RELATIVE_PATH FILE_NAME "${CMAKE_SOURCE_DIR}/resources" "${GLSL}")
    get_filename_component(FILE_EXT ${GLSL} EXT)
    if(${FILE_EXT} STREQUAL ".vert")
        set(SHADER_STAGE "SIGMA_ENGINE_VERTEX_SHADER")
    elseif(${FILE_EXT} STREQUAL ".frag")
        set(SHADER_STAGE "SIGMA_ENGINE_FRAGMENT_SHADER")
    elseif(${FILE_EXT} STREQUAL ".geom")
        set(SHADER_STAGE "SIGMA_ENGINE_GEOMETRY_SHADER")
    endif()
    set(SPIRV "${PROJECT_BINARY_DIR}/resources/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_BINARY_DIR}/shaders/"
        COMMAND glslc --target-env=opengl -D${SHADER_STAGE} -I "${CMAKE_SOURCE_DIR}/resources" -I "${CMAKE_CURRENT_SOURCE_DIR}/resources" ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL})
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach()
add_custom_target(shaders DEPENDS ${SPIRV_BINARY_FILES})
add_dependencies(simple-game shaders)

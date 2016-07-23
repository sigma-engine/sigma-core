file(GLOB_RECURSE SHADER_SOURCES . *.vert *.frag)
file(GLOB_RECURSE MATERIAL_SOURCES . *.mat)
file(GLOB_RECURSE MODEL_SOURCES . *.blend)

add_custom_target(editor-resources
    COMMAND shader-compiler --output="${CMAKE_BINARY_DIR}/data" ${SHADER_SOURCES}
    COMMAND material-compiler --output="${CMAKE_BINARY_DIR}/data" ${MATERIAL_SOURCES}
    COMMAND model-compiler --output="${CMAKE_BINARY_DIR}/data" ${MODEL_SOURCES}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    SOURCES ${SHADER_SOURCES} ${MATERIAL_SOURCES} ${MODEL_SOURCES})

add_dependencies(editor-resources model-compiler material-compiler shader-compiler)

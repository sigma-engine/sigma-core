file(GLOB_RECURSE SHADER_SOURCES . *.vert *.frag)
file(GLOB_RECURSE TEXTURE_SOURCES . *.bmp *.dds *.exr *.gif *.hdr *.ico *.iff *.jpg *.jpeg *.tiff *.png)
file(GLOB_RECURSE MATERIAL_SOURCES . *.mat)
file(GLOB_RECURSE MODEL_SOURCES . *.blend *.fbx *.obj)


add_custom_target(test-game-resources
    COMMAND ${CONAN_BIN_DIRS_SIGMA-FIVE-ENGINE}/shader-compiler --output="${CMAKE_BINARY_DIR}/data" ${SHADER_SOURCES}
    COMMAND ${CONAN_BIN_DIRS_SIGMA-FIVE-ENGINE}/texture-compiler --output="${CMAKE_BINARY_DIR}/data" ${TEXTURE_SOURCES}
    COMMAND ${CONAN_BIN_DIRS_SIGMA-FIVE-ENGINE}/material-compiler --output="${CMAKE_BINARY_DIR}/data" ${MATERIAL_SOURCES}
    COMMAND ${CONAN_BIN_DIRS_SIGMA-FIVE-ENGINE}/model-compiler --output="${CMAKE_BINARY_DIR}/data" ${MODEL_SOURCES}
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    SOURCES ${SHADER_SOURCES} ${TEXTURE_SOURCES} ${MATERIAL_SOURCES} ${MODEL_SOURCES})

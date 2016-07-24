set(SHADER_COMPILER shader-compiler)
set(TEXTURE_COMPILER texture-compiler)
set(MATERIAL_COMPILER material-compiler)
set(MODEL_COMPILER model-compiler)

macro(sigma_five_setup)
    file(COPY ${CONAN_SIGMA-FIVE-ENGINE_ROOT}/data DESTINATION ${CMAKE_BINARY_DIR})
    if(DEFINED CONAN_BIN_DIRS_SIGMA-FIVE-ENGINE)
        set(SHADER_COMPILER "${CONAN_BIN_DIRS_SIGMA-FIVE-ENGINE}/${SHADER_COMPILER}")
        set(TEXTURE_COMPILER "${CONAN_BIN_DIRS_SIGMA-FIVE-ENGINE}/${TEXTURE_COMPILER}")
        set(MATERIAL_COMPILER "${CONAN_BIN_DIRS_SIGMA-FIVE-ENGINE}/${MATERIAL_COMPILER}")
        set(MODEL_COMPILER "${CONAN_BIN_DIRS_SIGMA-FIVE-ENGINE}/${MODEL_COMPILER}")
    endif()
endmacro(sigma_five_setup)

function(add_resource_package package_name package_root_path)
    set(package_root_path "${CMAKE_CURRENT_LIST_DIR}/${package_root_path}")
    file(GLOB_RECURSE SHADER_SOURCES ${package_root_path} *.vert *.frag)
    file(GLOB_RECURSE TEXTURE_SOURCES ${package_root_path} *.bmp *.dds *.exr *.gif *.hdr *.ico *.iff *.jpg *.jpeg *.tiff *.png)
    file(GLOB_RECURSE MATERIAL_SOURCES ${package_root_path} *.mat)
    file(GLOB_RECURSE MODEL_SOURCES ${package_root_path} *.blend *.fbx *.obj)

    add_custom_target(${package_name}
        COMMAND ${SHADER_COMPILER} --output="${CMAKE_BINARY_DIR}/data" ${SHADER_SOURCES}
        COMMAND ${TEXTURE_COMPILER} --output="${CMAKE_BINARY_DIR}/data" ${TEXTURE_SOURCES}
        COMMAND ${MATERIAL_COMPILER} --output="${CMAKE_BINARY_DIR}/data" ${MATERIAL_SOURCES}
        COMMAND ${MODEL_COMPILER} --output="${CMAKE_BINARY_DIR}/data" ${MODEL_SOURCES}
        WORKING_DIRECTORY ${package_root_path}
        SOURCES ${SHADER_SOURCES} ${TEXTURE_SOURCES} ${MATERIAL_SOURCES} ${MODEL_SOURCES})
    if(NOT DEFINED CONAN_BIN_DIRS_SIGMA-FIVE-ENGINE)
        add_dependencies(${package_name} shader-compiler texture-compiler material-compiler model-compiler)
    endif()
endfunction(add_resource_package)

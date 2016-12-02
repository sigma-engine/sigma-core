set(SHADER_COMPILER scshader)
set(TEXTURE_COMPILER sctexture)
set(MATERIAL_COMPILER scmaterial)
set(MODEL_COMPILER scmodel)
set(REFLECTION_COMPILER screflect)

macro(sigma_setup)
    file(COPY ${CONAN_SIGMA-ENGINE_ROOT}/data DESTINATION ${CMAKE_BINARY_DIR})
    if(DEFINED CONAN_BIN_DIRS_SIGMA-ENGINE)
        set(SHADER_COMPILER "${CONAN_BIN_DIRS_SIGMA-ENGINE}/${SHADER_COMPILER}")
        set(TEXTURE_COMPILER "${CONAN_BIN_DIRS_SIGMA-ENGINE}/${TEXTURE_COMPILER}")
        set(MATERIAL_COMPILER "${CONAN_BIN_DIRS_SIGMA-ENGINE}/${MATERIAL_COMPILER}")
        set(MODEL_COMPILER "${CONAN_BIN_DIRS_SIGMA-ENGINE}/${MODEL_COMPILER}")
        set(REFLECTION_COMPILER "${CONAN_BIN_DIRS_SIGMA-ENGINE}/${REFLECTION_COMPILER}")
    endif()
endmacro(sigma_setup)

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
    if(NOT DEFINED CONAN_BIN_DIRS_SIGMA-ENGINE)
        add_dependencies(${package_name} scshader sctexture scmaterial scmodel)
    endif()
endfunction(add_resource_package)

function(add_reflection_sources target)
    get_target_property(target_sources ${target} SOURCES)
    add_custom_target(${target}_reflection
        COMMAND ${REFLECTION_COMPILER} ${target_sources}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        SOURCES ${target_sources})
    add_dependencies(${target} ${target}_reflection)
endfunction(add_reflection_sources)

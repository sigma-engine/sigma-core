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

    set(TIME_STAMPS)
    foreach(texture ${TEXTURE_SOURCES})
        file(RELATIVE_PATH texture_time_stamp ${package_root_path} ${texture})
        set(texture_time_stamp "${CMAKE_BINARY_DIR}/data/${texture_time_stamp}.stamp")
        add_custom_command(
            OUTPUT ${texture_time_stamp}
            COMMAND ${TEXTURE_COMPILER} --output="${CMAKE_BINARY_DIR}/data" ${texture}
            COMMAND ${CMAKE_COMMAND} -E touch ${texture_time_stamp}
            WORKING_DIRECTORY ${package_root_path}
            COMMENT ""
            SOURCES ${texture}
        )
        set(TIME_STAMPS ${TIME_STAMPS} ${texture_time_stamp})
    endforeach()

    foreach(shader ${SHADER_SOURCES})
        file(RELATIVE_PATH shader_time_stamp ${package_root_path} ${shader})
        set(shader_time_stamp "${CMAKE_BINARY_DIR}/data/${shader_time_stamp}.stamp")
        add_custom_command(
            OUTPUT ${shader_time_stamp}
            COMMAND ${SHADER_COMPILER} --output="${CMAKE_BINARY_DIR}/data" ${shader}
            COMMAND ${CMAKE_COMMAND} -E touch ${shader_time_stamp}
            WORKING_DIRECTORY ${package_root_path}
            COMMENT ""
            SOURCES ${shader}
        )
        set(TIME_STAMPS ${TIME_STAMPS} ${shader_time_stamp})
    endforeach()

    foreach(material ${MATERIAL_SOURCES})
        file(RELATIVE_PATH material_time_stamp ${package_root_path} ${material})
        set(material_time_stamp "${CMAKE_BINARY_DIR}/data/${material_time_stamp}.stamp")
        add_custom_command(
            OUTPUT ${material_time_stamp}
            COMMAND ${MATERIAL_COMPILER} --output="${CMAKE_BINARY_DIR}/data" ${material}
            COMMAND ${CMAKE_COMMAND} -E touch ${material_time_stamp}
            WORKING_DIRECTORY ${package_root_path}
            COMMENT ""
            SOURCES ${material}
        )
        set(TIME_STAMPS ${TIME_STAMPS} ${material_time_stamp})
    endforeach()

    foreach(model ${MODEL_SOURCES})
        file(RELATIVE_PATH model_time_stamp ${package_root_path} ${model})
        set(model_time_stamp "${CMAKE_BINARY_DIR}/data/${model_time_stamp}.stamp")
        add_custom_command(
            OUTPUT ${model_time_stamp}
            COMMAND ${MODEL_COMPILER} --output="${CMAKE_BINARY_DIR}/data" ${model}
            COMMAND ${CMAKE_COMMAND} -E touch ${model_time_stamp}
            WORKING_DIRECTORY ${package_root_path}
            COMMENT ""
            SOURCES ${model}
        )
        set(TIME_STAMPS ${TIME_STAMPS} ${model_time_stamp})
    endforeach()

    add_custom_target(${package_name} ALL DEPENDS ${TIME_STAMPS})
    
    if(NOT DEFINED CONAN_BIN_DIRS_SIGMA-ENGINE)
        add_dependencies(${package_name} sctexture scshader scmaterial scmodel)
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

set(TEXTURE_EXTENSIONS ".tiff" ".tif" ".jpg" ".jpeg" ".jpe" ".jif" ".jfif" ".jfi" ".png" ".hdr")
set(CUBEMAP_EXTENSIONS ".cub")
set(MATERIAL_EXTENSIONS ".smat")
set(EFFECT_EXTENSIONS ".eff")
set(MODEL_EXTENSIONS ".3ds" ".dae" ".fbx" ".ifc-step" ".ase" ".dxf" ".hmp" ".md2"
                     ".md3" ".md5" ".mdc" ".mdl" ".nff" ".ply" ".stl" ".x" ".obj"
                     ".opengex" ".smd" ".lwo" ".lxo" ".lws" ".ter" ".ac3d" ".ms3d"
                     ".cob" ".q3bsp" ".xgl" ".csm" ".bvh" ".b3d" ".ndo" ".q3d"
                     ".gltf" ".blend" ".3mf")

function(generate_meta_data generated_source_files)
    set(gen_list)
    set(TEMPLATE_REGEX "//[ \t]*TEMPLATE[ \t]*\\([ \t]*([a-z/A-Z_0-9]+(\\.[a-zA-Z_]+)?)[ \t]*\\)[ \t]*")
    foreach(header_file ${ARGN})
        file(RELATIVE_PATH header_file "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/${header_file}")

        set(output_file "${CMAKE_BINARY_DIR}/${header_file}")
        get_filename_component(dir "${output_file}" DIRECTORY)
        get_filename_component(name "${output_file}" NAME_WE)
        get_filename_component(ext "${output_file}" EXT)
        set(generated_meta_file "${dir}/${name}${ext}.meta")
        file(READ "${CMAKE_SOURCE_DIR}/${header_file}" file_text)

        string(REGEX MATCHALL ${TEMPLATE_REGEX} templates "${file_text}")
        if(templates)
            add_custom_command(
                OUTPUT "${generated_meta_file}"
                COMMAND python2 ARGS "${CMAKE_SOURCE_DIR}/tools/sreflect/sreflect.py" --source-directory "${CMAKE_SOURCE_DIR}" --build-directory "${CMAKE_BINARY_DIR}" --file "${CMAKE_SOURCE_DIR}/${header_file}" generate
                MAIN_DEPENDENCY "${CMAKE_SOURCE_DIR}/${header_file}"
                DEPENDS "${CMAKE_SOURCE_DIR}/tools/sreflect/sreflect.py"
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            )
            list(APPEND gen_list "${generated_meta_file}")

            foreach(template_file ${templates})
                STRING(REGEX REPLACE ${TEMPLATE_REGEX} "\\1" template_file "${template_file}")
                get_filename_component(template_type "${template_file}" NAME)

                if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${template_file}.j2")
                    set(template_file "${CMAKE_CURRENT_SOURCE_DIR}/${template_file}")
                else()
                    set(template_file "${CMAKE_SOURCE_DIR}/tools/sreflect/${template_file}")
                endif()


                set(generated_file "${dir}/${name}.${template_type}")
                set(template_file "${template_file}.j2")

                add_custom_command(
                    OUTPUT "${generated_file}"
                    COMMAND python2 ARGS "${CMAKE_SOURCE_DIR}/tools/sreflect/sreflect.py" --source-directory "${CMAKE_SOURCE_DIR}" --build-directory "${CMAKE_BINARY_DIR}" --file "${CMAKE_SOURCE_DIR}/${header_file}" template --template-file "${template_file}"
                    MAIN_DEPENDENCY "${generated_meta_file}"
                    DEPENDS "${template_file}" "${CMAKE_SOURCE_DIR}/tools/sreflect/sreflect.py"
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                )
                list(APPEND gen_list "${generated_file}")
            endforeach()
        endif()
    endforeach()

    set(${generated_source_files} ${gen_list} PARENT_SCOPE)
endfunction()

function(add_resources RESOURCE_TARGET RESOURCE_PACKAGE_DIRECTORY)
    if(NOT EXISTS "${RESOURCE_PACKAGE_DIRECTORY}")
        set(RESOURCE_PACKAGE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${RESOURCE_PACKAGE_DIRECTORY}")
    endif()

    target_include_directories(${RESOURCE_TARGET} PUBLIC "${RESOURCE_PACKAGE_DIRECTORY}")

    target_include_directories(${RESOURCE_TARGET} PUBLIC ${RESOURCE_PACKAGE_DIRECTORY})
    get_target_property(RESOURCE_INCLUDE_DIRECTORIES1 ${RESOURCE_TARGET} INCLUDE_DIRECTORIES)
    get_target_property(RESOURCE_INCLUDE_DIRECTORIES2 ${RESOURCE_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
    set(RESOURCE_INCLUDE_DIRECTORIES ${RESOURCE_INCLUDE_DIRECTORIES1} ${RESOURCE_INCLUDE_DIRECTORIES2})
    list(REMOVE_DUPLICATES RESOURCE_INCLUDE_DIRECTORIES)
    set(RESOURCE_INCLUDE_ARGUMENTS "-I \"${RESOURCE_PACKAGE_DIRECTORY}\"")
    foreach(INCLUDE_DIRECTORY ${RESOURCE_INCLUDE_DIRECTORIES})
        list(APPEND RESOURCE_INCLUDE_ARGUMENTS "-I \"${INCLUDE_DIRECTORY}\"")
    endforeach()

    set(RESOURCE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/data")
    file(GLOB_RECURSE SOURCE_FILES RELATIVE "${RESOURCE_PACKAGE_DIRECTORY}" "${RESOURCE_PACKAGE_DIRECTORY}/*.*")
    list(REMOVE_DUPLICATES SOURCE_FILES)

    set(OUTPUT_RESOURCES)
    foreach(SOURCE_FILE ${SOURCE_FILES})
        get_filename_component(SOURCE_DIRECTORY "${SOURCE_FILE}" DIRECTORY)
        get_filename_component(SOURCE_NAME "${SOURCE_FILE}" NAME_WE)
        get_filename_component(SOURCE_EXTENTION "${SOURCE_FILE}" EXT)

        # Skip files in hidden folders
        if("${SOURCE_DIRECTORY}" MATCHES ".*\\..*")
            continue()
        endif()

        if("${SOURCE_EXTENTION}" IN_LIST TEXTURE_EXTENSIONS)
            set(RESOURCE_TYPE "texture")
            set(RESOURCE_COMMAND texcc)
            set(RESOURCE_COMMAND_DEPENDS texcc)
        elseif("${SOURCE_EXTENTION}" IN_LIST CUBEMAP_EXTENSIONS)
            set(RESOURCE_TYPE "cubemap")
            set(RESOURCE_COMMAND cubcc)
            set(RESOURCE_COMMAND_DEPENDS cubcc)
        elseif("${SOURCE_EXTENTION}" IN_LIST MATERIAL_EXTENSIONS)
            set(RESOURCE_TYPE "material")
            set(RESOURCE_COMMAND mtlcc)
            set(RESOURCE_COMMAND_DEPENDS mtlcc)
        elseif("${SOURCE_EXTENTION}" IN_LIST EFFECT_EXTENSIONS)
            set(RESOURCE_TYPE "post_process_effect")
            set(RESOURCE_COMMAND effcc)
            set(RESOURCE_COMMAND_DEPENDS effcc)
        elseif("${SOURCE_EXTENTION}" IN_LIST MODEL_EXTENSIONS)
            set(RESOURCE_TYPE "blueprint")
            set(RESOURCE_COMMAND mdlcc)
            set(RESOURCE_COMMAND_DEPENDS mdlcc)
        elseif("${SOURCE_EXTENTION}" STREQUAL ".vert")
            set(RESOURCE_TYPE "vertex")
            set(RESOURCE_COMMAND shdcc ${RESOURCE_INCLUDE_ARGUMENTS})
            set(RESOURCE_COMMAND_DEPENDS shdcc)
        elseif("${SOURCE_EXTENTION}" STREQUAL ".frag")
            set(RESOURCE_TYPE "fragment")
            set(RESOURCE_COMMAND shdcc ${RESOURCE_INCLUDE_ARGUMENTS})
            set(RESOURCE_COMMAND_DEPENDS shdcc)
        elseif("${SOURCE_EXTENTION}" STREQUAL ".geom")
            set(RESOURCE_TYPE "geometry")
            set(RESOURCE_COMMAND shdcc ${RESOURCE_INCLUDE_ARGUMENTS})
            set(RESOURCE_COMMAND_DEPENDS shdcc)
        else()
            continue()
        endif()

        if("${SOURCE_DIRECTORY}" STREQUAL "")
            set(RESOURCE_IDENTIFIER "${RESOURCE_TYPE}/${SOURCE_NAME}")
        else()
            set(RESOURCE_IDENTIFIER "${RESOURCE_TYPE}/${SOURCE_DIRECTORY}/${SOURCE_NAME}")
        endif()

        string(REGEX REPLACE "[^a-zA-Z0-9]" "_" DEPENDENCY_NAME ${RESOURCE_IDENTIFIER})
        set(RESOURCE_DEPENDENCY_FILE "${RESOURCE_OUTPUT_DIRECTORY}/${RESOURCE_IDENTIFIER}${SOURCE_EXTENTION}.dependency")
        include("${RESOURCE_DEPENDENCY_FILE}" OPTIONAL)

        set(RESOURCE_COMMAND ${RESOURCE_COMMAND} -H "${RESOURCE_PACKAGE_DIRECTORY}" -B "${CMAKE_BINARY_DIR}" -c "${RESOURCE_PACKAGE_DIRECTORY}/${SOURCE_FILE}")
        list(APPEND OUTPUT_RESOURCES "${RESOURCE_OUTPUT_DIRECTORY}/${RESOURCE_IDENTIFIER}")
        add_custom_command(
            OUTPUT "${RESOURCE_OUTPUT_DIRECTORY}/${RESOURCE_IDENTIFIER}"
            COMMAND ${RESOURCE_COMMAND}
            COMMAND ${RESOURCE_COMMAND} -M
            MAIN_DEPENDENCY "${RESOURCE_PACKAGE_DIRECTORY}/${SOURCE_FILE}"
            DEPENDS ${RESOURCE_COMMAND_DEPENDS} ${${DEPENDENCY_NAME}_DEPENDS}
            WORKING_DIRECTORY "${RESOURCE_PACKAGE_DIRECTORY}"
            COMMENT "Compiling ${SOURCE_FILE}"
        )
    endforeach()
    add_custom_target(${RESOURCE_TARGET}-resources ALL DEPENDS ${OUTPUT_RESOURCES})
endfunction()

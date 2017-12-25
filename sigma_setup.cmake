set(TEXTURE_EXTENSIONS ".tiff" ".tif" ".jpg" ".jpeg" ".jpe" ".jif" ".jfif" ".jfi" ".png" ".hdr")
set(CUBEMAP_EXTENSIONS ".cub")
set(TECHNIQUE_EXTENSIONS ".stech")
set(MATERIAL_EXTENSIONS ".smat")
set(EFFECT_EXTENSIONS ".eff")
set(MODEL_EXTENSIONS ".3ds" ".dae" ".fbx" ".ifc-step" ".ase" ".dxf" ".hmp" ".md2"
                     ".md3" ".md5" ".mdc" ".mdl" ".nff" ".ply" ".stl" ".x" ".obj"
                     ".opengex" ".smd" ".lwo" ".lxo" ".lws" ".ter" ".ac3d" ".ms3d"
                     ".cob" ".q3bsp" ".xgl" ".csm" ".bvh" ".b3d" ".ndo" ".q3d"
                     ".gltf" ".3mf")

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

    file(WRITE "${RESOURCE_OUTPUT_DIRECTORY}/texture/database.lock" "")
    file(WRITE "${RESOURCE_OUTPUT_DIRECTORY}/cubemap/database.lock" "")
    file(WRITE "${RESOURCE_OUTPUT_DIRECTORY}/shader/database.lock" "")
    file(WRITE "${RESOURCE_OUTPUT_DIRECTORY}/technique/database.lock" "")
    file(WRITE "${RESOURCE_OUTPUT_DIRECTORY}/material/database.lock" "")
    file(WRITE "${RESOURCE_OUTPUT_DIRECTORY}/static_mesh/database.lock" "")
    file(WRITE "${RESOURCE_OUTPUT_DIRECTORY}/post_process_effect/database.lock" "")
    file(WRITE "${RESOURCE_OUTPUT_DIRECTORY}/blueprint/database.lock" "")

    #set(OUTPUT_RESOURCES)
    # foreach(SOURCE_FILE ${SOURCE_FILES})
    #     get_filename_component(SOURCE_DIRECTORY "${SOURCE_FILE}" DIRECTORY)
    #     get_filename_component(SOURCE_NAME "${SOURCE_FILE}" NAME_WE)
    #     get_filename_component(SOURCE_EXTENTION "${SOURCE_FILE}" EXT)
    #
    #     # Skip files in hidden folders
    #     if("${SOURCE_DIRECTORY}" MATCHES ".*\\..*")
    #         continue()
    #     endif()
    #
    #     if("${SOURCE_EXTENTION}" IN_LIST TEXTURE_EXTENSIONS)
    #         set(RESOURCE_TYPES "texture")
    #         set(texture_COMMAND texcc)
    #         set(texture_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" IN_LIST CUBEMAP_EXTENSIONS)
    #         set(RESOURCE_TYPES "cubemap")
    #         set(cubemap_COMMAND cubcc)
    #         set(cubemap_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" STREQUAL ".vert")
    #         set(RESOURCE_TYPES "vertex")
    #         set(vertex_COMMAND effcc ${RESOURCE_INCLUDE_ARGUMENTS})
    #         set(vertex_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" STREQUAL ".tesc")
    #         set(RESOURCE_TYPES "tessellation_control")
    #         set(tessellation_control_COMMAND effcc ${RESOURCE_INCLUDE_ARGUMENTS})
    #         set(tessellation_control_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" STREQUAL ".tese")
    #         set(RESOURCE_TYPES "tessellation_evaluation")
    #         set(tessellation_evaluation_COMMAND effcc ${RESOURCE_INCLUDE_ARGUMENTS})
    #         set(tessellation_evaluation_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" STREQUAL ".geom")
    #         set(RESOURCE_TYPES "geometry")
    #         set(geometry_COMMAND effcc ${RESOURCE_INCLUDE_ARGUMENTS})
    #         set(geometry_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" STREQUAL ".frag")
    #         set(RESOURCE_TYPES "fragment")
    #         set(fragment_COMMAND effcc ${RESOURCE_INCLUDE_ARGUMENTS})
    #         set(fragment_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" IN_LIST TECHNIQUE_EXTENSIONS)
    #         set(RESOURCE_TYPES "technique")
    #         set(technique_COMMAND effcc)
    #         set(technique_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" IN_LIST MATERIAL_EXTENSIONS)
    #         set(RESOURCE_TYPES "material")
    #         set(material_COMMAND effcc)
    #         set(material_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" IN_LIST MODEL_EXTENSIONS)
    #         set(RESOURCE_TYPES "static_mesh")
    #         set(static_mesh_COMMAND mdlcc)
    #         set(static_mesh_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" IN_LIST EFFECT_EXTENSIONS)
    #         set(RESOURCE_TYPES "post_process_effect")
    #         set(post_process_effect_COMMAND effcc)
    #         set(post_process_effect_COMMAND_DEPENDS)
    #     elseif("${SOURCE_EXTENTION}" STREQUAL ".blend")
    #         set(RESOURCE_TYPES "blueprint")
    #         set(blueprint_COMMAND blender "${RESOURCE_PACKAGE_DIRECTORY}/${SOURCE_FILE}" --background --python "${CMAKE_SOURCE_DIR}/tools/mdlcc/blendcc.py" --)
    #         set(blueprint_COMMAND_DEPENDS "${CMAKE_SOURCE_DIR}/tools/mdlcc/blendcc.py")
    #     else()
    #         continue()
    #     endif()
    #
    #     foreach(RESOURCE_TYPE ${RESOURCE_TYPES})
    #         if("${SOURCE_DIRECTORY}" STREQUAL "")
    #             set(RESOURCE_IDENTIFIER "${RESOURCE_TYPE}/${SOURCE_NAME}")
    #         else()
    #             set(RESOURCE_IDENTIFIER "${RESOURCE_TYPE}/${SOURCE_DIRECTORY}/${SOURCE_NAME}")
    #         endif()
    #
    #         string(REGEX REPLACE "[^a-zA-Z0-9]" "_" DEPENDENCY_NAME ${RESOURCE_IDENTIFIER})
    #         set(RESOURCE_DEPENDENCY_FILE "${RESOURCE_OUTPUT_DIRECTORY}/${RESOURCE_IDENTIFIER}${SOURCE_EXTENTION}.dependency")
    #         include("${RESOURCE_DEPENDENCY_FILE}" OPTIONAL)
    #
    #         set(${RESOURCE_TYPE}_COMMAND ${${RESOURCE_TYPE}_COMMAND} -H "${RESOURCE_PACKAGE_DIRECTORY}" -B "${CMAKE_BINARY_DIR}" -c "${RESOURCE_PACKAGE_DIRECTORY}/${SOURCE_FILE}")
    #         list(APPEND OUTPUT_${RESOURCE_TYPE} "${RESOURCE_DEPENDENCY_FILE}")
    #         add_custom_command(
    #             OUTPUT "${RESOURCE_DEPENDENCY_FILE}"
    #             COMMAND ${${RESOURCE_TYPE}_COMMAND}
    #             MAIN_DEPENDENCY "${RESOURCE_PACKAGE_DIRECTORY}/${SOURCE_FILE}"
    #             DEPENDS ${${RESOURCE_TYPE}_COMMAND_DEPENDS} ${${DEPENDENCY_NAME}_DEPENDS}
    #             WORKING_DIRECTORY "${RESOURCE_PACKAGE_DIRECTORY}"
    #             COMMENT "Compiling ${SOURCE_FILE}"
    #         )
    #     endforeach()
    # endforeach()
    # add_custom_target(${RESOURCE_TARGET}-textures ALL DEPENDS texcc ${OUTPUT_texture})
    # add_custom_target(${RESOURCE_TARGET}-cubemaps ALL DEPENDS cubcc ${RESOURCE_TARGET}-textures ${OUTPUT_cubemap})
    # add_custom_target(${RESOURCE_TARGET}-shaders ALL DEPENDS effcc ${OUTPUT_vertex} ${OUTPUT_tessellation_control} ${OUTPUT_tessellation_evaluation} ${OUTPUT_geometry} ${OUTPUT_fragment})
    # add_custom_target(${RESOURCE_TARGET}-techniques ALL DEPENDS effcc ${RESOURCE_TARGET}-shaders ${OUTPUT_technique})
    # add_custom_target(${RESOURCE_TARGET}-materials ALL DEPENDS effcc ${RESOURCE_TARGET}-textures ${RESOURCE_TARGET}-cubemaps ${RESOURCE_TARGET}-techniques ${OUTPUT_material})
    # add_custom_target(${RESOURCE_TARGET}-static-meshes ALL DEPENDS mdlcc ${RESOURCE_TARGET}-materials ${OUTPUT_static_mesh})
    # add_custom_target(${RESOURCE_TARGET}-post-process-effects ALL DEPENDS effcc ${RESOURCE_TARGET}-textures ${RESOURCE_TARGET}-cubemaps ${RESOURCE_TARGET}-techniques ${RESOURCE_TARGET}-static-meshes ${OUTPUT_post_process_effect})
    # add_custom_target(${RESOURCE_TARGET}-blueprints ALL DEPENDS ${RESOURCE_TARGET}-post-process-effects ${OUTPUT_blueprint})
    # add_custom_command(
    #    OUTPUT "${RESOURCE_OUTPUT_DIRECTORY}/graphics/settings"
    #    COMMAND pakcc -H "${RESOURCE_PACKAGE_DIRECTORY}" -B "${CMAKE_BINARY_DIR}"
    #    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    # )
    # add_custom_target(${RESOURCE_TARGET}-package ALL DEPENDS pakcc ${RESOURCE_TARGET}-blueprints ) #"${RESOURCE_OUTPUT_DIRECTORY}/graphics/settings"
endfunction()

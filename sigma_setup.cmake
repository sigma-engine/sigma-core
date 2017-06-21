set(SCOMPILER scompiler)

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

function(add_resources target)
    # https://cmake.org/cmake/help/v3.0/module/CMakeParseArguments.html
    set(options)
    set(oneValueArgs PACKAGE_ROOT)
    set(multiValueArgs)
    cmake_parse_arguments(package "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    if(package_PACKAGE_ROOT)
        set(package_PACKAGE_ROOT "${CMAKE_CURRENT_LIST_DIR}/${package_PACKAGE_ROOT}")
    else()
        set(package_PACKAGE_ROOT "${CMAKE_CURRENT_LIST_DIR}")
    endif()

    if(package_UNPARSED_ARGUMENTS)
        target_include_directories(${target} PUBLIC ${package_PACKAGE_ROOT})

        get_target_property(include_dirs ${target} INCLUDE_DIRECTORIES)
        set(include_args -I${package_PACKAGE_ROOT})
        foreach(inc ${include_dirs})
            list(APPEND include_args -I${inc})
        endforeach()
        list(REMOVE_DUPLICATES include_args)

        set(resource_files ${package_UNPARSED_ARGUMENTS})
        list(REMOVE_DUPLICATES resource_files)

        set(stamp_files)
        set(TEXTURE_EXTENSIONS
            ".tiff" ".tif" ".jpg" ".jpeg" ".jpe" ".jif" ".jfif" ".jfi" ".png"
            ".hdr")
        set(CUBEMAP_EXTENSIONS ".cub")
        set(SHADER_EXTENSIONS ".vert" ".frag" ".geom")
        set(EFFECT_EXTENSIONS ".eff")
        set(MATERIAL_EXTENSIONS ".smat")
        set(MODEL_EXTENSIONS ".3ds" ".blend" ".dae" ".fbx" ".ifc-step" ".ase"
            ".dxf" ".hmp" ".md2" ".md3" ".md5" ".mdc" ".mdl" ".nff" ".ply"
            ".stl" ".x" ".obj" ".opengex" ".smd" ".lwo" ".lxo" ".lws" ".ter"
            ".ac3d" ".ms3d" ".cob" ".q3bsp" ".xgl" ".csm" ".bvh" ".b3d" ".ndo"
            ".q3d" ".gltf" ".3mf")
       set(RESOURCE_EXTENSIONS ${TEXTURE_EXTENSIONS} ${CUBEMAP_EXTENSIONS}
           ${SHADER_EXTENSIONS} ${EFFECT_EXTENSIONS} ${MATERIAL_EXTENSIONS}
           ${MODEL_EXTENSIONS})

        foreach(resource_file ${resource_files})
            file(RELATIVE_PATH rel_resource_file ${package_PACKAGE_ROOT} ${resource_file})

            get_filename_component(dir "${rel_resource_file}" DIRECTORY)
            get_filename_component(name "${rel_resource_file}" NAME_WE)
            get_filename_component(ext "${rel_resource_file}" EXT)

            if("${dir}" STREQUAL "")
                set(package_dir "${package_PACKAGE_ROOT}")
                set(data_dir "${CMAKE_BINARY_DIR}/data")
            else()
                set(package_dir "${package_PACKAGE_ROOT}/${dir}")
                set(data_dir "${CMAKE_BINARY_DIR}/data/${dir}")
            endif()
            set(generated_stamp_file "${data_dir}/${name}${ext}.stamp")

            if(${ext} IN_LIST RESOURCE_EXTENSIONS)
                set(extra_DEPENDS)
                if(EXISTS "${data_dir}/${name}${ext}.stamp")
                    file(READ "${data_dir}/${name}${ext}.stamp" extra_DEPENDS)
                endif()

                list(APPEND stamp_files "${generated_stamp_file}")
                add_custom_command(
                    OUTPUT "${generated_stamp_file}"
                    COMMAND ${SCOMPILER} ARGS --output "${CMAKE_BINARY_DIR}/data" ${include_args} "${package_dir}/${name}${ext}"
                    MAIN_DEPENDENCY "${package_dir}/${name}${ext}"
                    DEPENDS ${extra_DEPENDS}
                    WORKING_DIRECTORY ${package_PACKAGE_ROOT}
                    COMMENT "Compiling ${rel_resource_file}"
                )
            endif()
        endforeach()

        add_custom_target(${target}-resources ALL DEPENDS ${stamp_files})
        add_custom_command(OUTPUT always_rebuild COMMAND cmake -E echo COMMENT "")
        add_dependencies(${target}-resources scompiler)

    endif()
endfunction()

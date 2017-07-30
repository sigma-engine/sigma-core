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

    set(data_root "${CMAKE_BINARY_DIR}/data")
    set(output_files)

    if(package_UNPARSED_ARGUMENTS)
        target_include_directories(${target} PUBLIC ${package_PACKAGE_ROOT})

        get_target_property(include_dirs ${target} INCLUDE_DIRECTORIES)
        set(include_args "-I \"${package_PACKAGE_ROOT}\"")
        foreach(inc ${include_dirs})
            list(APPEND include_args "-I \"${inc}\"")
        endforeach()
        list(REMOVE_DUPLICATES include_args)


        set(resource_files ${package_UNPARSED_ARGUMENTS})
        list(REMOVE_DUPLICATES resource_files)

        foreach(resource_file ${resource_files})
            file(RELATIVE_PATH rel_resource_file ${package_PACKAGE_ROOT} ${resource_file})

            get_filename_component(dir "${rel_resource_file}" DIRECTORY)
            get_filename_component(name "${rel_resource_file}" NAME_WE)
            get_filename_component(ext "${rel_resource_file}" EXT)

            if(NOT "${dir}" STREQUAL "")
                set(dir "${dir}/")
            endif()

            if(${ext} IN_LIST TEXTURE_EXTENSIONS)
                set(resource_type "texture")
                set(resource_command texcc)
            elseif(${ext} IN_LIST CUBEMAP_EXTENSIONS)
                set(resource_type "cubemap")
                set(resource_command cubcc)
            elseif(${ext} IN_LIST MATERIAL_EXTENSIONS)
                set(resource_type "material")
                set(resource_command mtlcc)
            elseif(${ext} IN_LIST EFFECT_EXTENSIONS)
                set(resource_type "post_process_effect")
                set(resource_command effcc)
            elseif(${ext} IN_LIST MODEL_EXTENSIONS)
                set(resource_type "blueprint")
                set(resource_command mdlcc)
            elseif(${ext} STREQUAL ".vert")
                set(resource_type "vertex")
                set(resource_command shdcc ${include_args})
            elseif(${ext} STREQUAL ".frag")
                set(resource_type "fragment")
                set(resource_command shdcc ${include_args})
            elseif(${ext} STREQUAL ".geom")
                set(resource_type "geometry")
                set(resource_command shdcc ${include_args})
            else()
                continue()
            endif()

            set(resource_command ${resource_command} -H "${package_PACKAGE_ROOT}" -B "${CMAKE_BINARY_DIR}" -c "${resource_file}")

            set(rid "${resource_type}/${dir}${name}")
            string(REGEX REPLACE "[^a-zA-Z0-9]" "_" deps_name ${rid})

            set(deps_file "${data_root}/${rid}${ext}.deps")
            include(${deps_file} OPTIONAL)

            list(APPEND output_files "${data_root}/${rid}")
            list(APPEND output_files "${deps_file}")
            add_custom_command(
                OUTPUT ${data_root}/${rid}
                COMMAND ${resource_command}
                MAIN_DEPENDENCY ${resource_file}
                DEPENDS texcc cubcc shdcc mtlcc effcc ${${deps_name}_deps} ${deps_file}
                WORKING_DIRECTORY ${package_PACKAGE_ROOT}
                COMMENT "Compiling ${rel_resource_file}"
            )

            add_custom_command(
                OUTPUT ${deps_file}
                COMMAND ${resource_command} -M
                MAIN_DEPENDENCY ${resource_file}
                DEPENDS texcc cubcc shdcc mtlcc effcc
                WORKING_DIRECTORY ${package_PACKAGE_ROOT}
                COMMENT "Generating dependencies for ${rel_resource_file}"
            )

        endforeach()

        add_custom_target(${target}-resources ALL DEPENDS ${output_files})
        add_dependencies(${target}-resources texcc cubcc shdcc mtlcc effcc)
    endif()
endfunction()

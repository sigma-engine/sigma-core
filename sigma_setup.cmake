set(SCOMPILER scompiler)

macro(sigma_setup)
    file(COPY ${CONAN_SIGMA-ENGINE_ROOT}/data DESTINATION ${CMAKE_BINARY_DIR})
    if(DEFINED CONAN_BIN_DIRS_SIGMA-ENGINE)
        set(SHADER_COMPILER "${CONAN_BIN_DIRS_SIGMA-ENGINE}/${SHADER_COMPILER}")
        set(TEXTURE_COMPILER "${CONAN_BIN_DIRS_SIGMA-ENGINE}/${TEXTURE_COMPILER}")
        set(MATERIAL_COMPILER "${CONAN_BIN_DIRS_SIGMA-ENGINE}/${MATERIAL_COMPILER}")
        set(MODEL_COMPILER "${CONAN_BIN_DIRS_SIGMA-ENGINE}/${MODEL_COMPILER}")
        set(REFLECTION_COMPILER "${CONAN_BIN_DIRS_SIGMA-ENGINE}/${REFLECTION_COMPILER}")
    endif()
endmacro()

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
                    COMMAND python2 ARGS "${CMAKE_SOURCE_DIR}/tools/sreflect/sreflect.py" --source-directory "${CMAKE_SOURCE_DIR}" --build-directory "${CMAKE_BINARY_DIR}" --file "${CMAKE_SOURCE_DIR}/${header_file}" template --template-file="${template_file}"
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

        add_custom_target(${target}-resources ALL DEPENDS  ${resource_files})

        add_custom_command(
            TARGET ${target}-resources
            COMMAND ${SCOMPILER} ARGS --output="${CMAKE_BINARY_DIR}/data" ${include_args} ${resource_files}
            WORKING_DIRECTORY ${package_PACKAGE_ROOT}
            DEPENDS always_rebuild
            COMMENT "Compiling ${target} resources..."
        )
        add_custom_command(OUTPUT always_rebuild COMMAND cmake -E echo COMMENT "")

        add_dependencies(${target}-resources scompiler)

    endif()
endfunction()

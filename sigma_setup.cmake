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

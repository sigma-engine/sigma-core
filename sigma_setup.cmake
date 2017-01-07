set(SHADER_COMPILER scshader)
set(TEXTURE_COMPILER sctexture)
set(MATERIAL_COMPILER scmaterial)
set(MODEL_COMPILER scmodel)
set(REFLECTION_COMPILER screflect)

set(SIGMA_SHADER_INCLUDE_REGEX "#[ \t]*include[ \t]*[<\"]([^>\"]*)[>\"]\n")

function(get_file_depends filename output)
    file(READ ${filename} source)
    string(REGEX MATCHALL ${SIGMA_SHADER_INCLUDE_REGEX} included_files "${source}")
    set(my_deps)
    foreach(include_file ${included_files})
        string(REGEX REPLACE ${SIGMA_SHADER_INCLUDE_REGEX} "\\1" inc "${include_file}")
        foreach(inc_path ${ARGN})
            if(EXISTS "${inc_path}/${inc}")
                set(include_path "${inc_path}/${inc}")
            endif()
        endforeach()
        if(include_path)
            list(APPEND my_deps ${include_path})
        else()
            message(ERORR "'${inc}' does not exist.")
        endif()
    endforeach()
    foreach(dep ${my_deps})
        if(NOT dep IN_LIST ${output})
            get_file_depends(${dep} ${output} ${ARGN})
        endif()
        set(${output} ${${output}} ${dep} PARENT_SCOPE)
    endforeach()
endfunction()

function(generate_type_regex_and_glob name)
    set(globs)
    foreach(stype ${ARGN})
        list(APPEND globs *.${stype})
    endforeach()
    string(REPLACE ";" "|" regx "${ARGN}")
    set(SIGMA_ENGINE_${name}_GLOB ${globs} PARENT_SCOPE)
    set(SIGMA_ENGINE_${name}_REGEX ".*\\.(${regx})" PARENT_SCOPE)
endfunction()

generate_type_regex_and_glob(TEXTURE png jpeg jpg jpeg jpe jif jfif jfi tiff tif)
generate_type_regex_and_glob(SHADER vert frag tesc geom comp)
generate_type_regex_and_glob(MATERIAL mat eff)
generate_type_regex_and_glob(MODEL 3ds blend dae fbx ifc-step ase dxf hmp md2 md3 md5 mdc mdl nff ply stl x obj opengex smd lwo lxo lws ter ac3d ms3d cob q3bsp xgl csm bvh b3d ndo q3d assbin gltf 3mf)

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
        get_target_property(include_dirs ${target} INCLUDE_DIRECTORIES)
        set(include_args -I${package_PACKAGE_ROOT})
        foreach(inc ${include_dirs})
            list(APPEND include_args -I${inc})
        endforeach()
        list(REMOVE_DUPLICATES include_args)

        set(TIME_STAMPS)
        set(resource_files ${package_UNPARSED_ARGUMENTS})
        list(REMOVE_DUPLICATES resource_files)

        set(texture_files ${resource_files})
        list(FILTER texture_files INCLUDE REGEX ${SIGMA_ENGINE_TEXTURE_REGEX})
        foreach(texture ${texture_files})
            file(RELATIVE_PATH texture_time_stamp ${package_PACKAGE_ROOT} ${texture})
            set(texture_time_stamp "${CMAKE_BINARY_DIR}/data/${texture_time_stamp}.stamp")

            get_filename_component(directory ${texture_time_stamp} DIRECTORY)
            file(MAKE_DIRECTORY ${directory})

            add_custom_command(
                OUTPUT "${texture_time_stamp}"
                COMMAND ${TEXTURE_COMPILER} ARGS --output="${CMAKE_BINARY_DIR}/data" "${texture}"
                COMMAND ${CMAKE_COMMAND} ARGS -E touch "${texture_time_stamp}"
                MAIN_DEPENDENCY "${texture}"
                WORKING_DIRECTORY "${package_PACKAGE_ROOT}"
                COMMENT ""
                SOURCES "${texture}"
            )
            set(TIME_STAMPS ${TIME_STAMPS} ${texture_time_stamp})
        endforeach()

        set(shader_files ${resource_files})
        list(FILTER shader_files INCLUDE REGEX ${SIGMA_ENGINE_SHADER_REGEX})
        foreach(shader ${shader_files})
            file(RELATIVE_PATH shader_time_stamp ${package_PACKAGE_ROOT} ${shader})
            set(shader_time_stamp "${CMAKE_BINARY_DIR}/data/${shader_time_stamp}.stamp")

            get_filename_component(directory ${shader_time_stamp} DIRECTORY)
            file(MAKE_DIRECTORY ${directory})

            get_file_depends(${shader} include_deps ${include_dirs})

            add_custom_command(
                OUTPUT "${shader_time_stamp}"
                COMMAND ${SHADER_COMPILER} ARGS --output="${CMAKE_BINARY_DIR}/data" ${include_args} "${shader}"
                COMMAND ${CMAKE_COMMAND} ARGS -E touch "${shader_time_stamp}"
                MAIN_DEPENDENCY "${shader}"
                DEPENDS ${include_deps}
                WORKING_DIRECTORY "${package_PACKAGE_ROOT}"
                COMMENT ""
                SOURCES "${shader}"
            )
            set(TIME_STAMPS ${TIME_STAMPS} ${shader_time_stamp})
        endforeach()

        set(material_files ${resource_files})
        list(FILTER material_files INCLUDE REGEX ${SIGMA_ENGINE_MATERIAL_REGEX})
        foreach(material ${material_files})
            file(RELATIVE_PATH material_time_stamp ${package_PACKAGE_ROOT} ${material})
            set(material_time_stamp "${CMAKE_BINARY_DIR}/data/${material_time_stamp}.stamp")

            get_filename_component(directory ${material_time_stamp} DIRECTORY)
            file(MAKE_DIRECTORY ${directory})

            add_custom_command(
                OUTPUT "${material_time_stamp}"
                COMMAND ${MATERIAL_COMPILER} ARGS --output="${CMAKE_BINARY_DIR}/data" "${material}"
                COMMAND ${CMAKE_COMMAND} ARGS -E touch "${material_time_stamp}"
                MAIN_DEPENDENCY "${material}"
                WORKING_DIRECTORY "${package_PACKAGE_ROOT}"
                COMMENT ""
                SOURCES "${material}"
            )
            set(TIME_STAMPS ${TIME_STAMPS} ${material_time_stamp})
        endforeach()

        set(model_files ${resource_files})
        list(FILTER model_files INCLUDE REGEX ${SIGMA_ENGINE_MODEL_REGEX})
        foreach(model ${MODEL_SOURCES})
            file(RELATIVE_PATH model_time_stamp ${package_PACKAGE_ROOT} ${model})
            set(model_time_stamp "${CMAKE_BINARY_DIR}/data/${model_time_stamp}.stamp")

            get_filename_component(directory ${model_time_stamp} DIRECTORY)
            file(MAKE_DIRECTORY ${directory})

            add_custom_command(
                OUTPUT "${model_time_stamp}"
                COMMAND ${MODEL_COMPILER} ARGS --output="${CMAKE_BINARY_DIR}/data" "${model}"
                COMMAND ${CMAKE_COMMAND} ARGS -E touch "${model_time_stamp}"
                MAIN_DEPENDENCY "${model}"
                WORKING_DIRECTORY "${package_PACKAGE_ROOT}"
                COMMENT ""
                SOURCES "${model}"
            )
            set(TIME_STAMPS ${TIME_STAMPS} ${model_time_stamp})
        endforeach()

        add_custom_target(${target}-resources ALL DEPENDS ${TIME_STAMPS})
        add_dependencies(${target} ${target}-resources)
        target_include_directories(${target} PUBLIC ${package_PACKAGE_ROOT})

        if(NOT DEFINED CONAN_BIN_DIRS_SIGMA-ENGINE)
            add_dependencies(${target}-resources sctexture scshader scmaterial scmodel)
        endif()
    endif()
endfunction()

function(add_reflection_sources target)
    get_target_property(target_sources ${target} SOURCES)
    add_custom_target(${target}_reflection
        COMMAND ${REFLECTION_COMPILER} ${target_sources}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        SOURCES ${target_sources})
    add_dependencies(${target} ${target}_reflection)
endfunction(add_reflection_sources)

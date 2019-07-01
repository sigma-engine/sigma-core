if (${CMAKE_HOST_SYSTEM_PROCESSOR} STREQUAL "AMD64")
	set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin/glslangValidator.exe")
else()
	set(GLSL_VALIDATOR "$ENV{VULKAN_SDK}/Bin32/glslangValidator.exe")
endif()

function(compile_glsl_sources TARGET_NAME)
	set(VULKAN_OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/vulkan")
	set(OPENGL_OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/opengl")
	make_directory("${VULKAN_OUTPUT_LOCATION}")
	make_directory("${OPENGL_OUTPUT_LOCATION}")

    get_target_property(TARGET_SOURES ${TARGET_NAME} SOURCES)
    foreach(SOURCE ${TARGET_SOURES})
        get_filename_component(SOURCE_EXT "${SOURCE}" EXT)
		get_filename_component(SOURCE_FILENAME "${SOURCE}" NAME_WE)
        if ("${SOURCE_EXT}" STREQUAL ".vert" OR "${SOURCE_EXT}" STREQUAL ".tesc" OR "${SOURCE_EXT}" STREQUAL ".tese" OR "${SOURCE_EXT}" STREQUAL ".geom" OR "${SOURCE_EXT}" STREQUAL ".frag" OR "${SOURCE_EXT}" STREQUAL ".comp")
            if(NOT EXISTS "${SOURCE}")
                set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
			endif()
			
			get_filename_component(SOURCE "${SOURCE}" ABSOLUTE)
			file(RELATIVE_PATH OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${SOURCE}")
			set(VULKAN_OUTPUT "${VULKAN_OUTPUT_LOCATION}/${OUTPUT_PATH}")
			set(OPENGL_OUTPUT "${OPENGL_OUTPUT_LOCATION}/${OUTPUT_PATH}")

			add_custom_command(OUTPUT "${VULKAN_OUTPUT}"
				COMMAND glslc "${SOURCE}" -o "${VULKAN_OUTPUT}" --target-env="vulkan1.1"
                DEPENDS "${SOURCE}"
			)

			add_custom_command(OUTPUT "${OPENGL_OUTPUT}"
				COMMAND glslc "${SOURCE}" -o "${OPENGL_OUTPUT}" -E
                DEPENDS "${SOURCE}"
			)
			
			target_sources(${TARGET_NAME} PRIVATE "${VULKAN_OUTPUT}" "${OPENGL_OUTPUT}")
        endif()
    endforeach()
endfunction()

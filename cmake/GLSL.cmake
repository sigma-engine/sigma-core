function(compile_glsl_sources TARGET_NAME)
	set(VULKAN_OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/vulkan")
	set(OPENGL_OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/opengl")
	make_directory("${VULKAN_OUTPUT_LOCATION}")
	make_directory("${OPENGL_OUTPUT_LOCATION}")

    get_target_property(TARGET_SOURES ${TARGET_NAME} SOURCES)
    foreach(SOURCE ${TARGET_SOURES})
        get_filename_component(SOURCE_EXT "${SOURCE}" EXT)
		get_filename_component(SOURCE_FILENAME "${SOURCE}" NAME_WE)

		if (SOURCE_EXT MATCHES ".*\\.glsl")
			string(REGEX REPLACE "\\.glsl" "" SHADER_STAGE "${SOURCE_EXT}")
			string(REGEX REPLACE "\\." "" SHADER_STAGE "${SHADER_STAGE}")
			
            if(NOT EXISTS "${SOURCE}")
                set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
			endif()
			
			get_filename_component(SOURCE "${SOURCE}" ABSOLUTE)
			file(RELATIVE_PATH OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}" "${SOURCE}")
			set(VULKAN_OUTPUT "${VULKAN_OUTPUT_LOCATION}/${OUTPUT_PATH}")
			set(OPENGL_OUTPUT "${OPENGL_OUTPUT_LOCATION}/${OUTPUT_PATH}")

			add_custom_command(OUTPUT "${VULKAN_OUTPUT}"
				COMMAND glslc -fshader-stage="${SHADER_STAGE}" "${SOURCE}" -o "${VULKAN_OUTPUT}" --target-env="vulkan1.1" -DSIGMA_VULKAN 
                DEPENDS "${SOURCE}"
			)

			add_custom_command(OUTPUT "${OPENGL_OUTPUT}"
				COMMAND glslc -fshader-stage="${SHADER_STAGE}" "${SOURCE}" -o "${OPENGL_OUTPUT}" -E -DSIGMA_OPENGL
                DEPENDS "${SOURCE}"
			)
			
			target_sources(${TARGET_NAME} PRIVATE "${VULKAN_OUTPUT}" "${OPENGL_OUTPUT}")
        endif()
    endforeach()
endfunction()

function(copy_texture_sources TARGET_NAME)
	set(TEXTURE_OUTPUT_LOCATION "${CMAKE_BINARY_DIR}/textures")
	make_directory("${TEXTURE_OUTPUT_LOCATION}")

    get_target_property(TARGET_SOURES ${TARGET_NAME} SOURCES)
    foreach(SOURCE ${TARGET_SOURES})
        get_filename_component(SOURCE_EXT "${SOURCE}" EXT)
		get_filename_component(SOURCE_FILENAME "${SOURCE}" NAME_WE)

		if (SOURCE_EXT MATCHES ".*\\.jpg")
            if(NOT EXISTS "${SOURCE}")
                set(SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE}")
			endif()
			
			get_filename_component(SOURCE "${SOURCE}" ABSOLUTE)
			file(RELATIVE_PATH OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/textures" "${SOURCE}")
			set(TEXTURE_OUTPUT "${TEXTURE_OUTPUT_LOCATION}/${OUTPUT_PATH}")
			message(STATUS "${TEXTURE_OUTPUT}")

			add_custom_command(OUTPUT "${TEXTURE_OUTPUT}"
				COMMAND "${CMAKE_COMMAND}" -E copy "${SOURCE}" "${TEXTURE_OUTPUT}"
                DEPENDS "${SOURCE}"
			)
			
			target_sources(${TARGET_NAME} PRIVATE "${TEXTURE_OUTPUT}")
        endif()
    endforeach()
endfunction()

cmake_minimum_required(VERSION 2.8.12)

function(add_plugin name)
	add_library(${name} MODULE ${ARGN})
	set_target_properties(${name}
		PROPERTIES
		PREFIX ""
		LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
		RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)
endfunction()

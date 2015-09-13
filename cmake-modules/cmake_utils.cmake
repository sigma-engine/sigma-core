set_property(GLOBAL PROPERTY USE_FOLDERS On)

#enable c++11 support
if(NOT MSVC) #TODO others??
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

function(source_group)
	foreach(file ${ARGN})
		#if file is an absolute path make it relitive to current source/build directory
		if(IS_ABSOLUTE ${file})
			string(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" file "${file}")
			string(REGEX REPLACE "${CMAKE_CURRENT_BINARY_DIR}/" "" file "${file}")
		endif()

		#only add if in current source directory
		if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${file}" AND NOT "${file}" MATCHES ".*\\.\\..*")
			#get the file extention
			get_filename_component(extention "${file}" EXT)

			#remove the name and extention of the file
			get_filename_component(file_path "${file}" DIRECTORY)

			#if it is a source file
			if("${extention}" MATCHES "\\.(C|M|c|c\\+\\+|cc|cpp|cxx|f|f90|for|fpp|ftn|m|mm|rc|def|r|odl|idl|hpj|bat)$")
				string(REGEX REPLACE "(source|sources|src)" "" file_path "${file_path}") #TODO others??
				set(file_path "Source Files/${file_path}")
			elseif("${extention}" MATCHES "\\.(h|hh|h\\+\\+|hm|hpp|hxx|in|txx|inl)$")
				string(REGEX REPLACE "(include|includes|inc)" "" file_path "${file_path}") #TODO others??
				set(file_path "Header Files/${file_path}")
			endif()

			#convert to the source group format (see cmake documentation for source_group)
			string(REGEX REPLACE "/" "\\\\" group "${file_path}")

			if(NOT ${group} STREQUAL "")
				#add the file to the source group
				_source_group(${group} FILES ${file})
			endif()
		endif()
	endforeach()
endfunction()

function(default_target_setup_ target)
	#mimic the source file folder structor in IDE virtual folders
	get_target_property(source_files ${target} SOURCES)
	source_group(${source_files})

	#mimic project layout in IDE virtual folders
	file(RELATIVE_PATH group ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
	get_filename_component(last_folder "${group}" NAME)
	if("${last_folder}" MATCHES ${target})
		string(REGEX REPLACE "/${last_folder}" "" group "${group}")
	endif()
	set_property(TARGET ${target} PROPERTY FOLDER ${group})

	#just add a few usefull defines
	if("${CMAKE_GENERATOR}" MATCHES "(Visual Studio|Xcode).*")
		target_compile_options(${target} PRIVATE -DCMAKE_IDE_GENERATOR)
	else()
		target_compile_options(${target} PRIVATE -DCMAKE_MAKEFILE_GENERATOR)
	endif()

	#get uppercase target name
	string(TOUPPER ${target} upper_target)

	#fix format for C/C++
	string(REGEX REPLACE "-" "_" upper_target "${upper_target}")

	#define debug
	target_compile_definitions(${target} PUBLIC -D$<$<CONFIG:Debug>:${upper_target}_DEBUG>)

	#check if it is a shared library or module
	get_target_property(target_type ${target} TYPE)
	if(${target_type} STREQUAL SHARED_LIBRARY OR ${target_type} STREQUAL MODULE_LIBRARY)
		#define export
		target_compile_definitions(${target} PRIVATE -D${upper_target}_EXPORT)
	endif()
endfunction()

function(add_library target)
	list(FIND ARGN "IMPORTED" imported_target)
	if(${imported_target} EQUAL -1)
		#add the library and cmake files
		_add_library(${target} ${ARGN})

		#setup some defalts
		default_target_setup_(${target})
	else()
		#add the library like normal
		_add_library(${target} ${ARGN})
	endif()
endfunction()

function(add_executable target)
	list(FIND ARGN "IMPORTED" imported_target)
	if(${imported_target} EQUAL -1)
		#add the executable and cmake files
		_add_executable(${target} ${ARGN})

		#setup some defalts
		default_target_setup_(${target})
	else()
		#add the executable like normal
		_add_executable(${target} ${ARGN})
	endif()
endfunction()
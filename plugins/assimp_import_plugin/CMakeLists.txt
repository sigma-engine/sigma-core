cmake_minimum_required(VERSION 2.8.12)
project(assimp_import_plugin)

find_package(assimp REQUIRED)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Qml Quick Widgets)

set(SOURCES
	src/assimp_import_plugin.cpp)

set(HEADERS
	include/assimp_import_plugin.hpp)

set(QML
	#TODO add files here
	)

qt5_wrap_ui(UI_HEADERS
	#TODO add files here
	)

qt5_add_resources(RESOURCES
	#TODO add files here
	)

add_library(assimp_import_plugin SHARED ${SOURCES} ${HEADERS} ${QML} ${UI_HEADERS} ${RESOURCES})

target_include_directories(assimp_import_plugin
PUBLIC
	include
PRIVATE
	${CMAKE_CURRENT_BINARY_DIR})

target_compile_definitions(assimp_import_plugin
PUBLIC
	-D$<$<CONFIG:Debug>:assimp_import_plugin_DEBUG>
PRIVATE
	-Dassimp_import_plugin_EXPORT)

if(NOT MSVC)
	target_compile_options(assimp_import_plugin PUBLIC -std=c++14 -pedantic)
endif()

target_link_libraries(assimp_import_plugin editor-core Qt5::Core Qt5::Qml Qt5::Quick ${ASSIMP_LIBRARIES})

set_target_properties(assimp_import_plugin
			PROPERTIES
			PREFIX ""
			LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins
			RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/plugins)

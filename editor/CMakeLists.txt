cmake_minimum_required(VERSION 2.8.12)
project(editor)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Qml OpenGL Quick Widgets)

set(SOURCES
	src/editor/application.cpp

	src/editor/component_manager.cpp
	src/editor/entity_manager.cpp
	src/editor/component_system_manager.cpp

	src/editor/widgets/MainWindow.cpp
	src/editor/widgets/GameView.cpp
	src/editor/widgets/GameViewRenderer.cpp
	src/editor/widgets/OpenGLWidget.cpp)
set(HEADERS
	include/editor/config.hpp
	include/editor/application.hpp
	include/editor/trackball_controller.hpp

	include/editor/component_manager.hpp
	include/editor/entity_manager.hpp
	include/editor/component_system_manager.hpp

	include/editor/widgets/MainWindow.hpp
	include/editor/widgets/GameView.hpp
	include/editor/widgets/GameViewRenderer.hpp
	include/editor/widgets/OpenGLWidget.hpp)

set(QML
	resources/ui/main.qml)

qt5_wrap_ui(UI_HEADERS
	resources/ui/MainWindow.ui)

qt5_add_resources(RESOURCES
	resources/resources.qrc)

add_library(editor-core SHARED ${SOURCES} ${HEADERS} ${UI_HEADERS})

target_include_directories(editor-core
	PUBLIC
	include
	PRIVATE
	${ASSIMP_INCLUDE_DIRS}
	${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(editor-core Qt5::Core Qt5::Qml Qt5::OpenGL Qt5::Quick Qt5::Widgets engine)

add_executable(editor WIN32 src/editor/main.cpp ${QML} ${RESOURCES})
target_link_libraries(editor editor-core)

add_subdirectory(plugins)

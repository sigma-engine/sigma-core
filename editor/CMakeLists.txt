cmake_minimum_required(VERSION 2.8.12)
project(editor)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core OpenGL Quick Widgets)
find_package(assimp REQUIRED)

set(SOURCES
    src/editor/main.cpp
    src/editor/application.cpp
    src/editor/trackball_controller.cpp

    src/editor/assimp_importer.cpp
    src/editor/entity_manager_model.cpp

    src/editor/widgets/MainWindow.cpp
    src/editor/widgets/GameView.cpp
    src/editor/widgets/OpenGLWidget.cpp)
set(HEADERS
    include/editor/application.hpp
    include/editor/trackball_controller.hpp

    include/editor/assimp_importer.hpp
    include/editor/entity_manager_model.hpp

    include/editor/widgets/MainWindow.hpp
    include/editor/widgets/GameView.hpp
    include/editor/widgets/OpenGLWidget.hpp)

set(QML
    resources/ui/main.qml)

qt5_wrap_ui(UI_HEADERS
    resources/ui/MainWindow.ui)

qt5_add_resources(RESOURCES
    resources/resources.qrc)

link_directories(${ASSIMP_LIBRARY_DIRS})

add_executable(editor WIN32 ${SOURCES} ${HEADERS} ${UI_HEADERS} ${QML} ${RESOURCES})

target_include_directories(editor
PUBLIC
    include
PRIVATE
    ${ASSIMP_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(editor Qt5::Core Qt5::OpenGL Qt5::Quick Qt5::Widgets engine ${ASSIMP_LIBRARIES})

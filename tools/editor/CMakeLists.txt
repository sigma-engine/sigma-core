cmake_minimum_required(VERSION 3.2)
project(editor)

add_subdirectory(resources)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Qml Quick Widgets)

set(SOURCES
    src/sigmafive/editor/main.cpp
    src/sigmafive/editor/MainWindow.cpp
    src/sigmafive/editor/dock_widget.cpp
    src/sigmafive/editor/editor.cpp
    src/sigmafive/editor/editor_game.cpp
    src/sigmafive/editor/opengl_widget.cpp
    src/sigmafive/editor/trackball_controller.cpp
)

set(HEADERS
    include/sigmafive/editor/MainWindow.hpp
    include/sigmafive/editor/dock_widget.hpp
    include/sigmafive/editor/editor.hpp
    include/sigmafive/editor/editor_game.hpp
    include/sigmafive/editor/opengl_widget.hpp
    include/sigmafive/editor/trackball_controller.hpp
)

set(QML
    resources/ui/main.qml
)

qt5_wrap_ui(UI_HEADERS
    resources/ui/MainWindow.ui
)

qt5_add_resources(RESOURCES
    resources/resources.qrc
)

add_executable(editor ${SOURCES} ${HEADERS} ${RESOURCES} ${QML} ${UI_HEADERS})

target_include_directories(editor
PUBLIC
    include
PRIVATE
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(editor
PUBLIC
    -D$<$<CONFIG:Debug>:SIGMA_FIVE_EDITOR_DEBUG>
)

target_compile_definitions(editor
PUBLIC
    -DCMAKE_SOURCE_DIR=boost::filesystem::path{\"${CMAKE_SOURCE_DIR}\"}
    -DCMAKE_BINARY_DIR=boost::filesystem::path{\"${CMAKE_BINARY_DIR}\"}
)

target_link_libraries(editor opengl gizmo Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets pthread ${CONAN_LIBS})

target_compile_options(editor PUBLIC -std=c++14)

add_dependencies(editor editor-resources)

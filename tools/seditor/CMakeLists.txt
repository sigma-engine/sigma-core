cmake_minimum_required(VERSION 3.2)
project(seditor)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Qml Quick Widgets)

set(SOURCES
    src/sigma/editor/main.cpp
    src/sigma/editor/MainWindow.cpp
    src/sigma/editor/dock_widget.cpp
    src/sigma/editor/editor.cpp
    src/sigma/editor/editor_game.cpp
    src/sigma/editor/opengl_widget.cpp
    src/sigma/editor/trackball_controller.cpp
)

set(HEADERS
    include/sigma/editor/MainWindow.hpp
    include/sigma/editor/dock_widget.hpp
    include/sigma/editor/editor.hpp
    include/sigma/editor/editor_game.hpp
    include/sigma/editor/opengl_widget.hpp
    include/sigma/editor/trackball_controller.hpp
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

add_executable(seditor ${SOURCES} ${HEADERS} ${RESOURCES} ${QML} ${UI_HEADERS})

target_include_directories(seditor
PUBLIC
    include
PRIVATE
    ${Qt5Core_INCLUDE_DIRS}
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_compile_definitions(seditor
PUBLIC
    -D$<$<CONFIG:Debug>:SIGMA_FIVE_EDITOR_DEBUG>
)

target_compile_definitions(seditor
PUBLIC
    -DCMAKE_SOURCE_DIR=boost::filesystem::path{\"${CMAKE_SOURCE_DIR}\"}
    -DCMAKE_BINARY_DIR=boost::filesystem::path{\"${CMAKE_BINARY_DIR}\"}
)

target_link_libraries(seditor sigma-engine sopengl gizmo Qt5::Core Qt5::Qml Qt5::Quick Qt5::Widgets pthread ${CONAN_LIBS})

add_resource_package(seditor-resources resources)
add_dependencies(seditor seditor-resources)

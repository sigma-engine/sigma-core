cmake_minimum_required(VERSION 2.8.12)
project(sigmafive-editor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)
find_package(assimp REQUIRED)

set(SOURCES
    src/main.cpp
    src/editor/widgets/MainWindow.cpp
    src/editor/widgets/OpenGLWidget.cpp
    src/editor/trackball_controller.cpp
)
set(HEADERS
    include/editor/widgets/MainWindow.hpp
    include/editor/widgets/OpenGLWidget.hpp
    include/editor/trackball_controller.hpp
)

qt5_wrap_ui(UI_HEADERS
    ui/MainWindow.ui
)

qt5_add_resources(UI_RESOURCES
    resources/resources.qrc
)

link_directories(${ASSIMP_LIBRARY_DIRS})

add_executable(sigmafive-editor WIN32 ${SOURCES} ${HEADERS} ${UI_HEADERS} ${UI_RESOURCES})
target_include_directories(sigmafive-editor
PUBLIC
    include
PRIVATE
    ${ASSIMP_INCLUDE_DIRS}
)

message("${ASSIMP_LIBRARIES}")
target_link_libraries(sigmafive-editor Qt5::Widgets sigmafive ${ASSIMP_LIBRARIES})
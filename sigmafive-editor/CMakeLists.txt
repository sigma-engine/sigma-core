cmake_minimum_required(VERSION 2.8.12)
project(sigmafive-editor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Widgets)
find_package(assimp REQUIRED)

set(SOURCES
    src/editor/main.cpp
    src/editor/application.cpp
    src/editor/trackball_controller.cpp

    src/editor/widgets/MainWindow.cpp
    src/editor/widgets/OpenGLWidget.cpp
)
set(HEADERS
    include/editor/application.hpp
    include/editor/trackball_controller.hpp

    include/editor/widgets/MainWindow.hpp
    include/editor/widgets/OpenGLWidget.hpp
)

qt5_wrap_ui(UI_HEADERS
    resources/ui/MainWindow.ui
)

qt5_add_resources(RESOURCES
    resources/resources.qrc
)

link_directories(${ASSIMP_LIBRARY_DIRS})

add_executable(sigmafive-editor WIN32 ${SOURCES} ${HEADERS} ${RESOURCES} ${UI_HEADERS})
target_include_directories(sigmafive-editor
PUBLIC
    include
PRIVATE
    ${ASSIMP_INCLUDE_DIRS}
)

target_link_libraries(sigmafive-editor Qt5::Core Qt5::Widgets sigmafive ${ASSIMP_LIBRARIES})

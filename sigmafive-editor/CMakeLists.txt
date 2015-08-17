cmake_minimum_required(VERSION 2.8.12)
project(sigmafive-editor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_library(assimp SHARED IMPORTED)
set_target_properties(assimp PROPERTIES IMPORTED_IMPLIB ${THIRDPARTY_DIR}/assimp/${CMAKE_STATIC_LIBRARY_PREFIX}assimp${CMAKE_STATIC_LIBRARY_SUFFIX})
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_DIR}/assimp/lib/${CMAKE_SHARED_LIBRARY_PREFIX}assimp${CMAKE_SHARED_LIBRARY_SUFFIX})

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

add_executable(sigmafive-editor WIN32 ${SOURCES} ${HEADERS} ${UI_HEADERS} ${UI_RESOURCES})
target_include_directories(sigmafive-editor
PUBLIC
    include
PRIVATE
    ${THIRDPARTY_DIR}/assimp/include
)
target_link_libraries(sigmafive-editor Qt5::Widgets sigmafive assimp)

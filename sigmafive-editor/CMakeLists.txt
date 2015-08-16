project(sigmafive-editor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_library(assimp SHARED IMPORTED)
set_target_properties(assimp PROPERTIES IMPORTED_IMPLIB ${THIRDPARTY_DIR}/assimp/${CMAKE_STATIC_LIBRARY_PREFIX}assimp${CMAKE_STATIC_LIBRARY_SUFFIX})
set_target_properties(assimp PROPERTIES IMPORTED_LOCATION ${THIRDPARTY_DIR}/assimp/lib/${CMAKE_SHARED_LIBRARY_PREFIX}assimp${CMAKE_SHARED_LIBRARY_SUFFIX})
include_directories(SYSTEM ${THIRDPARTY_DIR}/assimp/include)

include_directories(SYSTEM ../sigmafive-engine/include ../sigmafive-engine/generated)

set(SRCS
    src/main.cpp
    src/editor/widgets/MainWindow.cpp
    src/editor/widgets/OpenGLWidget.cpp
    src/editor/trackball_controller.cpp
)
set(HDRS
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

include_directories(include)
add_executable(sigmafive-editor WIN32 ${SRCS} ${HDRS} ${UI_HEADERS} ${UI_RESOURCES})
target_link_libraries(sigmafive-editor Qt5::Widgets sigmafive assimp)

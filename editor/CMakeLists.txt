project(sigma-five-editor)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

set(SRCS
    src/main.cpp
    src/editor/widgets/MainWindow.cpp
    src/editor/widgets/OpenGLWidget.cpp
)
set(HDRS
    include/editor/widgets/MainWindow.hpp
    include/editor/widgets/OpenGLWidget.hpp
)

qt5_wrap_ui(UI_HEADERS
    ui/MainWindow.ui
)
qt5_add_resources(UI_RESOURCES
    resources/resources.qrc
)

include_directories(include)
add_executable(sigma-five-editor WIN32 ${SRCS} ${HDRS} ${UI_HEADERS} ${UI_RESOURCES})
target_link_libraries(sigma-five-editor Qt5::Widgets)

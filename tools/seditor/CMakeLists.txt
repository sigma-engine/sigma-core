cmake_minimum_required(VERSION 3.2)
project(seditor)

set(CMAKE_AUTOMOC ON)
find_package(Qt5 COMPONENTS Core Qml Quick QuickControls2)

set(SOURCES
    src/main.cpp
)

set(HEADERS
)

set(QML
    ui/main.qml
    ui/Page1.qml
    ui/Page1Form.ui.qml
)

qt5_add_resources(RESOURCES
    qml.qrc
)

add_executable(seditor ${SOURCES} ${HEADERS} ${RESOURCES} ${QML} ${UI_HEADERS})

target_include_directories(seditor PUBLIC include PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(seditor sigma-core Qt5::Core Qt5::Qml Qt5::Quick Qt5::QuickControls2)

add_resource_package(seditor-resources resources)
add_dependencies(seditor seditor-resources)

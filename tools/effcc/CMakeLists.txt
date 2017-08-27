set(SOURCES
    src/main.cpp
)

add_executable(effcc ${SOURCES})
target_include_directories(effcc PUBLIC include)
target_link_libraries(effcc sigma-core CONAN_PKG::glslang)

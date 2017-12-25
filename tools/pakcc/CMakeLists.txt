set(SOURCES
    src/main.cpp
    src/package_texture.cpp
    src/package_cubemap.cpp
    src/package_shader.cpp
    src/package_technique.cpp
    src/package_material.cpp
    src/package_static_mesh.cpp
    src/package_post_process_effect.cpp
)

add_executable(pakcc ${SOURCES})
target_include_directories(pakcc PUBLIC include)
target_link_libraries(pakcc sigma-core CONAN_PKG::libpng CONAN_PKG::libtiff CONAN_PKG::libjpeg CONAN_PKG::glslang CONAN_PKG::assimp)

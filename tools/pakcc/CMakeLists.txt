add_executable(pakcc
    src/main.cpp
    src/package_cubemap.cpp
    src/package_material.cpp
    src/package_post_process_effect.cpp
    src/package_shader.cpp
    src/package_model.cpp
    src/package_technique.cpp
    src/package_texture.cpp
)

target_include_directories(pakcc
    PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}/include"
)

target_link_libraries(pakcc
    PUBLIC
    Boost::program_options
    Boost::wave
    PNG::PNG
    TIFF::TIFF
    jpeg
    assimp

    glslang
    HLSL
    OGLCompiler
    OSDependent
    sigma-core
    SPIRV
    SPVRemapper
)

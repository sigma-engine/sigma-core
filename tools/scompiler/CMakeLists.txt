set(SOURCES
    src/compiler_util.cpp
    src/assimp_converter.cpp
    src/cubemap_compiler.cpp
    src/main.cpp
    src/material_compiler.cpp
    src/model_compiler.cpp
    src/post_process_effect_compiler.cpp
    src/shader_compiler.cpp
    src/texture_compiler.cpp
    src/shader_validation.cpp
)

set(HEADERS
    include/compiler_util.hpp
    include/assimp_converter.hpp
    include/cubemap_compiler.hpp
    include/material_compiler.hpp
    include/model_compiler.hpp
    include/post_process_effect_compiler.hpp
    include/shader_compiler.hpp
    include/shader_technique_compiler.hpp
    include/texture_compiler.hpp
    include/shader_validation.hpp
    include/hdr_io.hpp
)

add_executable(scompiler ${SOURCES} ${HEADERS})
target_include_directories(scompiler PUBLIC include)
target_link_libraries(scompiler sigma-core ${CONAN_LIBS})

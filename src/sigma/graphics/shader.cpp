#include <sigma/graphics/shader.hpp>

namespace sigma {
namespace graphics {

    shader::shader(shader_type type, std::string code)
        : type(type)
        , source(code)
    {
    }
}
}

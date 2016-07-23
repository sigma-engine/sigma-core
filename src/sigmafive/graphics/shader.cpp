#include <sigmafive/graphics/shader.hpp>

namespace sigmafive {
namespace graphics {

    shader::shader(shader_type type, std::string code)
        : type(type)
        , source(code)
    {
    }
}
}

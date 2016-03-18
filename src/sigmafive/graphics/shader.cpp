#include <sigmafive/graphics/shader.hpp>

namespace sigmafive {
namespace graphics {

    shader::shader(shader_type type, std::string code)
        : type_(type)
        , source_(code)
    {
    }

    shader_type shader::type() const
    {
        return type_;
    }

    const std::string& shader::source() const
    {
        return source_;
    }
}
}

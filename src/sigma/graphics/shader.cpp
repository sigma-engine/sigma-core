#include <sigma/graphics/shader.hpp>

namespace sigma {
namespace graphics {
    shader::shader(std::weak_ptr<sigma::context> context, resource::key_type key)
        : resource::base_resource::base_resource(std::move(context), std::move(key))
    {
    }

    void shader::add_source(shader_type type, std::vector<unsigned char>&& spirv, shader_schema&& schema)
    {
        type_ = type;
        spirv_ = std::move(spirv);
        schema_ = std::move(schema);
    }

    shader_type shader::type() const
    {
        return type_;
    }

    const shader_schema& shader::schema() const
    {
        return schema_;
    }
}
}

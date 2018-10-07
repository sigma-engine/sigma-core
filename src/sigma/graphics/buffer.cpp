#include <sigma/graphics/buffer.hpp>

namespace sigma {
namespace graphics {
    buffer::buffer(std::weak_ptr<sigma::context> context, const resource::key_type& key)
        : resource::base_resource::base_resource(context, key)
    {
    }

    void from_json(const nlohmann::json& j, buffer_member& member)
    {
        member.name = j["name"].get<std::string>();
        member.offset = j["offset"].get<size_t>();
        std::string type = j["type"].get<std::string>();
        if (type == "float")
            member.type = buffer_type::FLOAT;
        else if (type == "vec2")
            member.type = buffer_type::VEC2;
        else if (type == "vec3")
            member.type = buffer_type::VEC3;
        else if (type == "vec4")
            member.type = buffer_type::VEC4;
        else if (type == "mat2")
            member.type = buffer_type::MAT2x2;
        else if (type == "mat3")
            member.type = buffer_type::MAT3x3;
        else if (type == "mat4")
            member.type = buffer_type::MAT4x4;
        else
            throw std::runtime_error("Buffer Type: " + type + " not supported!");
    }

    void from_json(const nlohmann::json& j, buffer_schema& schema)
    {
        for (const auto& member_j : j["members"]) {
            buffer_member member = member_j;
            schema.members.push_back(member);
        }
    }
}
}

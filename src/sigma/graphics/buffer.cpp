#include <sigma/graphics/buffer.hpp>

#include <sigma/util/glm_json.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace sigma {
namespace graphics {
    bool buffer_schema::merge(const buffer_schema& other)
    {
        if (type_name != other.type_name || descriptor_set != other.descriptor_set || binding_point != other.binding_point)
            return false;

        for (const auto& m : other.members) {
            auto it = members.find(m.first);
            if (it == members.end())
                members[m.first] = m.second;
            else if (it->second != m.second)
                return false;
        }

        size = std::max(other.size, size);

        return true;
    }

    buffer::buffer(std::weak_ptr<sigma::context> context, const resource::key_type& key)
        : resource::base_resource::base_resource(context, key)
    {
    }

    buffer::buffer(std::weak_ptr<sigma::context> context, const resource::key_type& key, const buffer_schema& schema)
        : resource::base_resource::base_resource(context, key)
        , schema_(schema)
        , buffer_(schema_.size, 0)
    {
    }

    const buffer_schema& buffer::schema() const
    {
        return schema_;
    }

    bool buffer::merge(const buffer_schema& other)
    {
        if (!schema_.merge(other))
            return false;
        buffer_.resize(schema_.size);
        return true;
    }

    void buffer::set(const std::string& member, float v)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::FLOAT);
        assert(!m.is_array);
        std::memcpy(buffer_.data() + m.offset, &v, sizeof(float));
    }

    void buffer::set(const std::string& member, size_t index, float v)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::FLOAT);
        assert(m.is_array);
        assert(index < m.count);
        std::memcpy(buffer_.data() + (m.offset + (index * m.stride)), &v, sizeof(float));
    }

    void buffer::set(const std::string& member, const glm::vec2& v)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::VEC2);
        assert(!m.is_array);
        std::memcpy(buffer_.data() + m.offset, glm::value_ptr(v), sizeof(glm::vec2));
    }

    void buffer::set(const std::string& member, size_t index, const glm::vec2& v)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::VEC2);
        assert(m.is_array);
        assert(index < m.count);
        std::memcpy(buffer_.data() + (m.offset + (index * m.stride)), glm::value_ptr(v), sizeof(glm::vec2));
    }

    void buffer::set(const std::string& member, const glm::vec3& v)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::VEC3);
        assert(!m.is_array);
        std::memcpy(buffer_.data() + m.offset, glm::value_ptr(v), sizeof(glm::vec3));
    }

    void buffer::set(const std::string& member, size_t index, const glm::vec3& v)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::VEC3);
        assert(m.is_array);
        assert(index < m.count);
        std::memcpy(buffer_.data() + (m.offset + (index * m.stride)), glm::value_ptr(v), sizeof(glm::vec3));
    }

    void buffer::set(const std::string& member, const glm::vec4& v)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::VEC4);
        assert(!m.is_array);
        std::memcpy(buffer_.data() + m.offset, glm::value_ptr(v), sizeof(glm::vec4));
    }

    void buffer::set(const std::string& member, size_t index, const glm::vec4& v)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::VEC4);
        assert(m.is_array);
        assert(index < m.count);
        std::memcpy(buffer_.data() + (m.offset + (index * m.stride)), glm::value_ptr(v), sizeof(glm::vec4));
    }

    void buffer::set(const std::string& member, const glm::mat3& mat)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::MAT3x3);
        assert(!m.is_array);
        size_t offset = m.offset;
        for (int i = 0; i < 3; ++i) {
            std::memcpy(buffer_.data() + offset, glm::value_ptr(mat[i]), sizeof(glm::vec3));
            offset += sizeof(glm::vec4);
        }
    }

    void buffer::set(const std::string& member, size_t index, const glm::mat3& mat)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::MAT3x3);
        assert(m.is_array);
        assert(index < m.count);
        size_t offset = m.offset + (index * m.stride);
        for (int i = 0; i < 3; ++i) {
            std::memcpy(buffer_.data() + offset, glm::value_ptr(mat[i]), sizeof(glm::vec3));
            offset += sizeof(glm::vec4);
        }
    }

    void buffer::set(const std::string& member, const glm::mat4& mat)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::MAT4x4);
        assert(!m.is_array);
        std::memcpy(buffer_.data() + m.offset, glm::value_ptr(mat), sizeof(glm::mat4));
    }

    void buffer::set(const std::string& member, size_t index, const glm::mat4& mat)
    {
        const auto& m = schema_.members.at(member);
        assert(m.type == buffer_type::MAT4x4);
        assert(m.is_array);
        assert(index < m.count);
        std::memcpy(buffer_.data() + (m.offset + (index * m.stride)), glm::value_ptr(mat), sizeof(glm::mat4));
    }

    void from_json(const nlohmann::json& j, buffer_member& member)
    {
        std::string type = j["type"].get<std::string>();
        auto array_it = j.find("array");

        member.name = j["name"].get<std::string>();
        member.offset = j["offset"].get<size_t>();
        member.is_array = array_it != j.end();

        if (member.is_array) {
            if (array_it->size() != 1)
                throw std::runtime_error("Multidimensional arrays are not supported!");
            member.count = (*array_it)[0].get<size_t>();
        }

        if (type == "float") {
            member.type = buffer_type::FLOAT;
            if (member.is_array)
                member.stride = sizeof(glm::vec4);
        } else if (type == "vec2") {
            member.type = buffer_type::VEC2;
            if (member.is_array)
                member.stride = sizeof(glm::vec4);
        } else if (type == "vec3") {
            member.type = buffer_type::VEC3;
            if (member.is_array)
                member.stride = sizeof(glm::vec4);
        } else if (type == "vec4") {
            member.type = buffer_type::VEC4;
            if (member.is_array)
                member.stride = sizeof(glm::vec4);
        } else if (type == "mat3") {
            member.type = buffer_type::MAT3x3;
            if (member.is_array)
                member.stride = 3 * sizeof(glm::vec4);
        } else if (type == "mat4") {
            member.type = buffer_type::MAT4x4;
            if (member.is_array)
                member.stride = sizeof(glm::mat4);
        } else {
            throw std::runtime_error("Buffer Type: " + type + " not supported!");
        }
    }

    void from_json(const nlohmann::json& j, buffer_schema& schema)
    {
        for (const auto& member_j : j["members"]) {
            buffer_member member = member_j;
            schema.members[member.name] = member;
        }
    }

    void from_json(const nlohmann::json& j, buffer& b)
    {
        const auto& schema = b.schema();
        for (const auto& item : j.items()) {
            const auto& value = item.value();
            const auto& key = item.key();
            auto it = schema.members.find(key);

            if (it == schema.members.end())
                continue;

            const auto& [name, member] = *it;

            switch (member.type) {
            case buffer_type::FLOAT: {
                if (member.is_array) {
                    for (size_t i = 0; i < value.size(); ++i) {
                        float v = value[i];
                        b.set(key, i, v);
                    }
                } else {
                    float v = item.value();
                    b.set(key, v);
                }
                break;
            }
            case buffer_type::VEC2: {
                if (member.is_array) {
                    for (size_t i = 0; i < value.size(); ++i) {
                        glm::vec2 v = value[i];
                        b.set(key, i, v);
                    }
                } else {
                    glm::vec2 v = item.value();
                    b.set(key, v);
                }
                break;
            }
            case buffer_type::VEC3: {
                if (member.is_array) {
                    for (size_t i = 0; i < value.size(); ++i) {
                        glm::vec3 v = value[i];
                        b.set(key, i, v);
                    }
                } else {
                    glm::vec3 v = item.value();
                    b.set(key, v);
                }
                break;
            }
            case buffer_type::VEC4: {
                if (member.is_array) {
                    for (size_t i = 0; i < value.size(); ++i) {
                        glm::vec4 v = value[i];
                        b.set(key, i, v);
                    }

                } else {
                    glm::vec4 v = item.value();
                    b.set(key, v);
                }
                break;
            }
            case buffer_type::MAT3x3: {
                if (member.is_array) {
                    for (size_t i = 0; i < value.size(); ++i) {
                        glm::mat3 v = value[i];
                        b.set(key, i, v);
                    }
                } else {
                    glm::mat3 v = item.value();
                    b.set(key, v);
                }
                break;
            }
            case buffer_type::MAT4x4: {
                if (member.is_array) {
                    for (size_t i = 0; i < value.size(); ++i) {
                        glm::mat4 v = value[i];
                        b.set(key, i, v);
                    }
                } else {
                    glm::mat4 v = item.value();
                    b.set(key, v);
                }
                break;
            }
            }
        }
    }
}
}

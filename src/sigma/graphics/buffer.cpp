#include <sigma/graphics/buffer.hpp>

#include <glm/gtc/type_ptr.hpp>

namespace sigma {
namespace graphics {
    bool buffer_schema::merge(const buffer_schema& other)
    {
        if (type_name != other.type_name || descriptor_set != other.descriptor_set || binding_point != other.binding_point)
        {
            return false;
        }

        for (const auto& m : other.members) {
            auto it = members.find(m.first);
            if (it == members.end())
            {
                members[m.first] = m.second;
            }
            else if (it->second != m.second)
            {
                return false;
            }
        }

        size = std::max(other.size, size);

        return true;
    }

    buffer::buffer(std::weak_ptr<sigma::context> context, resource::key_type key)
        : resource::base_resource::base_resource(std::move(context), std::move(key))
    {
    }

    buffer::buffer(std::weak_ptr<sigma::context> context, resource::key_type key, buffer_schema schema)
        : resource::base_resource::base_resource(std::move(context), std::move(key))
        , schema_(std::move(schema))
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
        {
            return false;
        }

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
}
}

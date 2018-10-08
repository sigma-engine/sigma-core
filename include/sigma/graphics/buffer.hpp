#ifndef SIGMA_GRAPHICS_BUFFER_HPP
#define SIGMA_GRAPHICS_BUFFER_HPP

#include <sigma/resource/resource.hpp>

#include <nlohmann/json.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <boost/serialization/unordered_map.hpp>
#include <boost/serialization/vector.hpp>

#include <unordered_map>

namespace sigma {
namespace graphics {
    enum class buffer_type {
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        MAT3x3,
        MAT4x4,
    };

    struct buffer_member {
        size_t offset = 0;
        buffer_type type;
        std::string name;
        bool is_array = false;
        size_t count = 0;
        size_t stride = 0;

        bool operator!=(const buffer_member& other) const
        {
            return offset != other.offset
                || type != other.type
                || name != other.name
                || is_array != other.is_array
                || count != other.count
                || stride != other.stride;
        }

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& offset;
            ar& type;
            ar& name;
            ar& is_array;
            ar& count;
            ar& stride;
        }
    };

    struct buffer_schema {
        size_t size;
        size_t descriptor_set;
        size_t binding_point;
        std::unordered_map<std::string, buffer_member> members;
        std::string type_name;
        std::string name;

        bool merge(const buffer_schema& other);

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& size;
            ar& descriptor_set;
            ar& binding_point;
            ar& members;
            ar& type_name;
            ar& name;
        }
    };

    class buffer : public resource::base_resource {
    public:
        buffer(std::weak_ptr<sigma::context> context, const resource::key_type& key);

        buffer(std::weak_ptr<sigma::context> context, const resource::key_type& key, const buffer_schema& schema);

        const buffer_schema& schema() const;

        bool merge(const buffer_schema& other);

        void set(const std::string& member, float v);
        void set(const std::string& member, size_t index, float v);

        void set(const std::string& member, const glm::vec2& v);
        void set(const std::string& member, size_t index, const glm::vec2& v);

        void set(const std::string& member, const glm::vec3& v);
        void set(const std::string& member, size_t index, const glm::vec3& v);

        void set(const std::string& member, const glm::vec4& v);
        void set(const std::string& member, size_t index, const glm::vec4& v);

        void set(const std::string& member, const glm::mat3& v);
        void set(const std::string& member, size_t index, const glm::mat3& v);

        void set(const std::string& member, const glm::mat4& v);
        void set(const std::string& member, size_t index, const glm::mat4& v);

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& schema_;
            ar& buffer_;
        }

    private:
        buffer_schema schema_;
        std::vector<uint8_t> buffer_;
    };

    // void to_json(nlohmann::json& j, const buffer_member& member);

    void from_json(const nlohmann::json& j, buffer_member& member);

    // void to_json(nlohmann::json& j, const buffer_schema& schema);

    void from_json(const nlohmann::json& j, buffer_schema& schema);

    // void to_json(nlohmann::json& j, const buffer& b);

    void from_json(const nlohmann::json& j, buffer& b);
}
}

REGISTER_RESOURCE(sigma::graphics::buffer, buffer, 1);
BOOST_CLASS_VERSION(sigma::graphics::buffer_schema, 1);
BOOST_CLASS_VERSION(sigma::graphics::buffer_member, 1);

#endif // SIGMA_GRAPHICS_BUFFER_HPP

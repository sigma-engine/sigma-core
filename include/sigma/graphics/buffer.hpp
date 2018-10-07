#ifndef SIGMA_GRAPHICS_BUFFER_HPP
#define SIGMA_GRAPHICS_BUFFER_HPP

#include <sigma/resource/resource.hpp>

#include <nlohmann/json.hpp>

#include <boost/serialization/vector.hpp>

#include <unordered_map>

namespace sigma {
namespace graphics {
    enum class buffer_type {
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        MAT2x2,
        MAT3x3,
        MAT4x4
    };

    struct buffer_member {
        size_t offset;
        buffer_type type;
        std::string name;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& offset;
            ar& type;
            ar& name;
        }
    };

    struct buffer_schema {
        size_t size;
        size_t descriptor_set;
        size_t binding_point;
        std::vector<buffer_member> members;
        std::string type_name;
        std::string name;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& size;
            ar& members;
            ar& type_name;
            ar& name;
        }
    };

    class buffer : public resource::base_resource {
    public:
        buffer(std::weak_ptr<sigma::context> context, const resource::key_type& key);

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& schema_;
        }

    private:
        buffer_schema schema_;
    };

    // void to_json(nlohmann::json& j, const buffer_member& member);

    void from_json(const nlohmann::json& j, buffer_member& member);

    // void to_json(nlohmann::json& j, const buffer_schema& schema);

    void from_json(const nlohmann::json& j, buffer_schema& schema);
}
}

REGISTER_RESOURCE(sigma::graphics::buffer, buffer, 1);

#endif // SIGMA_GRAPHICS_BUFFER_HPP

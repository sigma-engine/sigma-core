#ifndef SIGMA_GRAPHICS_STATIC_MESH_HPP
#define SIGMA_GRAPHICS_STATIC_MESH_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/resource/resource.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/glm_serialize.hpp>

#include <cereal/types/array.hpp>
#include <cereal/types/vector.hpp>

#include <array>
#include <cstddef>
#include <vector>

namespace sigma {
namespace graphics {
    class mesh_part {
    public:
        mesh_part();

        mesh_part(size_t start, size_t end, resource::handle_type<graphics::material> material);

        size_t start() const;

        size_t end() const;

        resource::handle_type<graphics::material> material() const;

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(start_, end_, material_);
        }

    private:
        std::size_t start_;
        std::size_t end_;
        resource::handle_type<graphics::material> material_;
    };

    class static_mesh : public resource::base_resource {
    public:
        struct vertex {
            glm::vec3 position;
            glm::vec3 normal;
            glm::vec3 bitangent;
            glm::vec3 tangent;
            glm::vec2 texcoord;

            template <class Archive>
            void serialize(Archive& ar)
            {
                ar(position, normal, bitangent, tangent, texcoord);
            }
        };

        typedef std::array<unsigned int, 3> triangle;

        static_mesh(std::weak_ptr<sigma::context> ctx, resource::key_type key);

        const std::vector<vertex>& vertices() const;

        std::vector<vertex>& vertices();

        const std::vector<triangle>& triangles() const;

        std::vector<triangle>& triangles();

        const std::vector<mesh_part>& parts() const;

        std::vector<mesh_part>& parts();

        float radius() const;

        void set_radius(float r);

        template <class Archive>
        void serialize(Archive& ar)
        {
            ar(radius_, vertices_, triangles_, parts_);
        }

    private:
        float radius_ = 0;
        std::vector<vertex> vertices_;
        std::vector<triangle> triangles_;
        std::vector<mesh_part> parts_;
    };
}
}

REGISTER_RESOURCE(sigma::graphics::static_mesh, static_mesh, 1);

#endif // SIGMA_GRAPHICS_STATIC_MESH_HPP

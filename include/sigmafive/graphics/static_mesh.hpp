#ifndef SIGMAFIVE_GRAPHICS_STATIC_MESH_HPP
#define SIGMAFIVE_GRAPHICS_STATIC_MESH_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/resource/resource.hpp>

#include <cppbr/math/vec2.hpp>
#include <cppbr/math/vec3.hpp>
#include <cppbr/math/mat4.hpp>

#include <array>
#include <vector>

namespace sigmafive {
    namespace graphics {
        class SIGMAFIVE_API static_mesh : public resource::resource {
        CPPBR_META_CLASS()
        public:
            struct vertex {
                float3 position;
                float3 normal;
                float3 tangent;
                float2 texcoord;
            };

            typedef std::array<unsigned int, 3> triangle;

            static_mesh(sigmafive::resource::identifier id);

            void set_data(const std::vector<vertex> &vertices, const std::vector<triangle> &triangles);

            std::vector<vertex> vertices() const { return vertices_; }

            std::vector<triangle> triangles() const { return triangles_; }

            ~static_mesh();

        private:
            std::vector<vertex> vertices_;
            std::vector<triangle> triangles_;
        };
    }
}

#endif //SIGMAFIVE_GRAPHICS_STATIC_MESH_HPP

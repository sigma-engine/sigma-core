#ifndef SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/component.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <array>
#include <vector>
#include <boost/serialization/array.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

namespace sigmafive {
    namespace graphics {
        class SIGMAFIVE_API static_mesh {
        public:
            struct vertex {
                glm::vec3 position;
                glm::vec3 normal;
                glm::vec3 tangent;
                glm::vec3 texcoord;
            };

            typedef std::array<unsigned int, 3> triangle;

            void set_data(std::vector<vertex> vertices, std::vector<triangle> triangles);

            const std::vector<vertex> &vertices() const;

            const std::vector<triangle> &triangles() const;

        private:
            friend class boost::serialization::access;
            template<class Archive>
            void serialize(Archive & ar, const unsigned int version) {
                ar & vertices_;
                ar & triangles_;
            }

            std::vector<vertex> vertices_;
            std::vector<triangle> triangles_;
        };
    }
}

namespace boost {
    namespace serialization {
        template<class Archive>
        void serialize(Archive & ar, sigmafive::graphics::static_mesh::vertex &vertex, const unsigned int version) {
            ar & vertex.position;
            ar & vertex.normal;
            ar & vertex.tangent;
            ar & vertex.texcoord;
        }
    }
}

SIGMAFIVE_COMPONENT(sigmafive::graphics::static_mesh)

#endif //SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_HPP

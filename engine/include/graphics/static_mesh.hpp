#ifndef SIGMAFIVE_GRAPHICS_STATIC_MESH_HPP
#define SIGMAFIVE_GRAPHICS_STATIC_MESH_HPP

#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/mat4.hpp>
#include <system/resource.hpp>

#include <array>
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/uuid/uuid.hpp>

namespace sigmafive {
	namespace graphics {
		class static_mesh : public system::resource {
            SIGMAFIVE_OBJECT()
		public:
			struct vertex {
				float3 position;
				float3 normal;
				float3 tangent;
				float2 texcoord;
			};

			typedef std::array<unsigned int,3> triangle;

			static_mesh();

			void set_data(const std::vector<vertex> &vertices, const std::vector<triangle> &triangles);

			std::vector<vertex> vertices() const { return vertices_; }
			std::vector<triangle> triangles() const { return triangles_; }

			~static_mesh();
		private:
			friend class boost::serialization::access;
			template <typename Archive>
			void serialize(Archive& ar, const unsigned int version) {
                ar & SIGMAFIVE_SERIALIZE_BASE(resource);
			}

			std::vector<vertex> vertices_;
			std::vector<triangle> triangles_;
		};
	}
}

#endif //SIGMAFIVE_GRAPHICS_STATIC_MESH_HPP

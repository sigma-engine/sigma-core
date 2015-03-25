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
		class static_mesh : public  system::resource {
		public:
			struct vertex {
				float3 position;
				float3 normal;
				float3 tangent;
				float2 texcoord;
			};

			typedef std::array<unsigned int,3> triangle;

			static_mesh();

			~static_mesh();
		private:
			std::vector<vertex> vertices_;
			std::vector<triangle> triangles_;
		};

		struct static_mesh_instance {
			boost::uuids::uuid resource_uuid;
			float4x4 model_matrix;
		};
	}
}

#endif //SIGMAFIVE_GRAPHICS_STATIC_MESH_HPP

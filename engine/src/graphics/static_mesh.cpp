#include <graphics/static_mesh.hpp>

namespace sigmafive {
	namespace graphics {
		static_mesh::static_mesh() {
		}

		static_mesh::~static_mesh() {
		}

		void static_mesh::set_data(const std::vector<static_mesh::vertex> &vertices, const std::vector<static_mesh::triangle> &triangles) {
			//TODO add checks
			vertices_ = std::move(vertices);
			triangles_ = std::move(triangles);
		}
	}
}

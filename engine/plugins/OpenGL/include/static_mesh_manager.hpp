#ifndef SIGMAFIVE_GRAPHICS_OPENGL_STATIC_MESH_MANAGER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_STATIC_MESH_MANAGER_HPP

#include <static_mesh.hpp>

#include <sigmafive/system/resource_manager.hpp>

#include <memory>
#include <unordered_map>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class static_mesh_manager {
			public:
				static_mesh_manager(system::resource_manager &resource_manager);

				opengl::static_mesh *get(boost::uuids::uuid mesh);
			private:
				system::resource_manager &resource_manager_;
				std::unordered_map<boost::uuids::uuid,std::unique_ptr<opengl::static_mesh>,boost::hash<boost::uuids::uuid>> static_meshes_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_STATIC_MESH_MANAGER_HPP

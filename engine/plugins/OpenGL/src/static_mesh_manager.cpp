#include <static_mesh_manager.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            static_mesh_manager::static_mesh_manager(resource::resource_manager &resource_manager)
                    : resource_manager_(resource_manager) {
            }

            opengl::static_mesh *static_mesh_manager::get(boost::uuids::uuid mesh) {
                auto it = static_meshes_.find(mesh);
                if (it != static_meshes_.end())
                    return it->second.get();
                auto sm = new opengl::static_mesh(resource_manager_.get<graphics::static_mesh>(mesh).get());
                static_meshes_[mesh] = std::unique_ptr<opengl::static_mesh>(sm);
                return sm;
            }
        }
    }
}

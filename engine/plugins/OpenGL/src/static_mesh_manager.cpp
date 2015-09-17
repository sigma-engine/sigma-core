#include <static_mesh_manager.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            opengl::static_mesh *static_mesh_manager::get(std::shared_ptr<graphics::static_mesh> mesh) {
                auto it = static_meshes_.find(mesh->uuid());
                if (it != static_meshes_.end())
                    return it->second.get();
                auto sm = new opengl::static_mesh(mesh.get());
                static_meshes_[mesh->uuid()] = std::unique_ptr<opengl::static_mesh>(sm);
                return sm;
            }
        }
    }
}

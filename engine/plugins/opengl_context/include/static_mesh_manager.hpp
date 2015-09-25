#ifndef SIGMAFIVE_GRAPHICS_OPENGL_STATIC_MESH_MANAGER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_STATIC_MESH_MANAGER_HPP

#include <static_mesh.hpp>
#include <sigmafive/resource/resource.hpp>

#include <memory>
#include <unordered_map>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            class static_mesh_manager {
            public:
                opengl::static_mesh *get(std::shared_ptr<graphics::static_mesh> mesh);
            private:
                std::unordered_map<resource::identifier, std::unique_ptr<opengl::static_mesh>> static_meshes_;
            };
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_STATIC_MESH_MANAGER_HPP

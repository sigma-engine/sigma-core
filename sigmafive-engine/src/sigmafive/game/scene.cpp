#include <sigmafive/game/scene.hpp>

namespace sigmafive {
    namespace game {
        void scene::add_static_mesh(std::uint32_t id, transform_component *transform, static_mesh_component *static_mesh) {
            static_meshes_.emplace(id,static_mesh_instance{transform,static_mesh});
        }

        void scene::remove_static_mesh(std::uint32_t id) {
            static_meshes_.erase(id);
        }

        std::queue<static_mesh_instance> scene::static_meshes() const {
            std::queue<static_mesh_instance> q;
            for(const auto &item: static_meshes_) {
                q.push(item.second);
            }

            return std::move(q);
        }
    }
}

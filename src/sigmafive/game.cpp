#include <sigmafive/game.hpp>

namespace sigmafive {

    game::game(component_manager<transform> &transforms, component_manager<graphics::static_mesh> &static_meshes)
        : transforms_(transforms_), static_meshes_(static_meshes) {
    }

    game::~game() {
    }

    const component_manager<transform> &game::transforms() const {
        return transforms_;
    }

    component_manager<transform> &game::transforms() {
        return transforms_;
    }

    component_manager<graphics::static_mesh> &game::static_meshes() {
        return static_meshes_;
    }

    const component_manager<graphics::static_mesh> &game::static_meshes() const {
        return static_meshes_;
    }
}

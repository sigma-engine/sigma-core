#ifndef SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_INSTANCE_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_INSTANCE_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/component.hpp>
#include <sigmafive/resource/identifier.hpp>

namespace sigmafive {
    namespace graphics {
        struct SIGMAFIVE_API static_mesh_instance {
            bool visible; // TODO check the odering on this
            resource::identifier static_mesh;
        };

        using static_mesh_instance_manager = basic_component_manager<static_mesh_instance>;
    }
}

SIGMAFIVE_COMPONENT(sigmafive::graphics::static_mesh_instance)

#endif //SIGMAFIVE_ENGINE_GRAPHICS_STATIC_MESH_INSTANCE_HPP

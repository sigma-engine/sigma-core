#include <sigmafive/engine.hpp>

#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>
#include <sigmafive/entity/default_component_pool.hpp>

namespace sigmafive {

    engine::engine(int &argc, char **argv) : plugin_manger(default_plugin_path()) {
        boost::filesystem::current_path(boost::filesystem::path{argv[0]}.parent_path());


        component_registry_.register_component<game::transform_component>(entity::default_component_pool<game::transform_component>::factory);
        component_registry_.register_component<game::static_mesh_component>(entity::default_component_pool<game::static_mesh_component>::factory);
    }

    boost::filesystem::path engine::default_plugin_path() {
#ifdef CMAKE_IDE_GENERATOR
#ifdef ENGINE_DEBUG
        return boost::filesystem::path("..")/"plugins"/"Debug";
#else
        return boost::filesystem::path("..")/"plugins"/"Release";
#endif
#else
        return boost::filesystem::path("..")/"plugins";
#endif
    }

    sigmafive::entity::component_registry *engine::component_registry() {
        return &component_registry_;
    }

    graphics::context_manager *engine::graphics_context_manager() {
        return &graphics_context_manager_;
    }

    system::window_manager *engine::window_manager() {
        return &window_manager_;
    }
}

EXPORT_CPPBR_META_CLASS(sigmafive::engine)

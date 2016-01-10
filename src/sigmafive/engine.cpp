#include <sigmafive/engine.hpp>

#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>
#include <sigmafive/entity/default_component_cache.hpp>

namespace sigmafive {

    engine::engine(int &argc, char **argv) : plugin_manger(default_plugin_path()) {
        boost::filesystem::current_path(boost::filesystem::path{argv[0]}.parent_path());

        plugin_manger.load_plugins(this);

        component_cache_factory_.register_cache<game::transform_component>(entity::default_component_cache<game::transform_component>::factory);
        component_cache_factory_.register_cache<game::static_mesh_component>(entity::default_component_cache<game::static_mesh_component>::factory);
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

    sigmafive::entity::component_cache_factory *engine::component_cache_factory() {
        return &component_cache_factory_;
    }

    graphics::context_manager *engine::graphics_context_manager() {
        return &graphics_context_manager_;
    }

    system::window_manager *engine::window_manager() {
        return &window_manager_;
    }
}

EXPORT_CPPBR_META_CLASS(sigmafive::engine)

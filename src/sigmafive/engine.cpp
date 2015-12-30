#include <sigmafive/engine.hpp>

#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>

#include <iostream>
#include <boost/range/iterator_range.hpp>

namespace sigmafive {

    engine::engine(int &argc, char **argv) {
        //TODO clean this up
        //this is a hack just to get things working
        for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(default_plugin_path()),
                                                      {})) {
            if (boost::dll::shared_library::suffix() == entry.path().extension()) {
                boost::dll::shared_library library{entry.path()};
                if (library.has("register_plugin")) {
                    plugins_.push_back(std::move(library));
                    plugins_.back().get<void(sigmafive::engine *)>("register_plugin")(this);
                }
                else {
                    std::cout << "warning: shared library in plugins folder that does not export a plugin." <<
                    std::endl;
                }
            }
        }

        component_registry_.register_component(game::transform_component::CLASS_ID,
                                               std::unique_ptr<game::transform_component_pool_factory>(
                                                       new game::transform_component_pool_factory{}));
        component_registry_.register_component(game::static_mesh_component::CLASS_ID,
                                               std::unique_ptr<game::static_mesh_component_pool_factory>(
                                                       new game::static_mesh_component_pool_factory{}));
    }

    boost::filesystem::path engine::default_plugin_path() {
#ifdef CMAKE_IDE_GENERATOR
#ifdef ENGINE_DEBUG
        return "../plugins/Debug";
#else
        return "../plugins/Release";
#endif
#else
        return boost::filesystem::path("../plugins");
#endif
    }

    game::component_registry *engine::component_registry() {
        return &component_registry_;
    }

    graphics::context_manager *engine::graphics_context_manager() {
        return &graphics_context_manager_;
    }

    system::window_manager *engine::window_manager() {
        return &window_manager_;
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::engine)
#include <sigmafive/engine.hpp>

#include <sigmafive/resource2/filesystem_package.hpp>

#include <sigmafive/game/game.hpp>
#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>
#include <sigmafive/entity/default_component_cache.hpp>

#include <boost/filesystem/operations.hpp>

namespace sigmafive {

    engine::engine(int &argc, char **argv)
        : resource_manager_(package_manager_) {
        boost::filesystem::current_path(boost::filesystem::path{argv[0]}.parent_path());

        package_manager_.add_package("filesystem",std::make_unique<resource2::filesystem_package>());
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

    system::window_manager &engine::window_manager() {
        return window_manager_;
    }

    graphics::context_manager &engine::graphics_context_manager() {
        return graphics_context_manager_;
    }

    resource2::package_manager &engine::package_manager() {
        return package_manager_;
    }

    resource2::resource_manager &engine::resource_manager() {
        return resource_manager_;
    }

    void engine::set_game(std::shared_ptr<game::game> game) {
        game_ = game;
    }

    std::shared_ptr<game::game> engine::game() {
        return game_;
    }
}

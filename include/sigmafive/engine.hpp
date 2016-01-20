#ifndef SIGMAFIVE_ENGINE_HPP
#define SIGMAFIVE_ENGINE_HPP

#include <sigmafive/config.hpp>

#include <sigmafive/system/window_manager.hpp>
#include <sigmafive/graphics/context_manager.hpp>

#include <sigmafive/resource2/package_manager.hpp>
#include <sigmafive/resource2/resource_manager.hpp>

#include <boost/filesystem/path.hpp>

namespace sigmafive {
    namespace game { class game; }
    
    class SIGMAFIVE_API engine {
    public:
        engine(int &argc, char **argv);

        static boost::filesystem::path default_plugin_path();

        system::window_manager &window_manager();

        graphics::context_manager &graphics_context_manager();

        resource2::package_manager &package_manager();

        resource2::resource_manager &resource_manager();

        void set_game(std::shared_ptr<game::game> game);

		std::shared_ptr<game::game> game();
    private:
        system::window_manager window_manager_;
        graphics::context_manager graphics_context_manager_;

        resource2::package_manager package_manager_;
        resource2::resource_manager resource_manager_;

        std::shared_ptr<game::game> game_;
    };
}

#endif //SIGMAFIVE_ENGINE_HPP

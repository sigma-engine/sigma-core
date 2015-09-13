#ifndef SIGMAFIVE_ENGINE_HPP
#define SIGMAFIVE_ENGINE_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/object.hpp>

#include <sigmafive/game/component_registry.hpp>

#include <sigmafive/system/window_manager.hpp>
#include <sigmafive/system/resource_manager.hpp>

#include <sigmafive/graphics/context_manager.hpp>

#include <vector>
#include <boost/dll/shared_library.hpp>

namespace sigmafive {
    class SIGMAFIVE_API engine : public object {
		SIGMAFIVE_CLASS()
    public:
        engine(int &argc,char **argv);

        static boost::filesystem::path default_plugin_path();

        system::resource_manager &resource_manager();

        game::component_registry &component_registry();

        graphics::context_manager &graphics_context_manager();

        system::window_manager &window_manager();
    private:
        //TODO hack
        std::vector<boost::dll::shared_library> plugins_;

        system::resource_manager resource_manager_;
        game::component_registry component_registry_;
        graphics::context_manager graphics_context_manager_;
        system::window_manager window_manager_;
    };
}

#endif //SIGMAFIVE_ENGINE_HPP

#ifndef SIGMAFIVE_ENGINE_HPP
#define SIGMAFIVE_ENGINE_HPP

#include <sigmafive/system/resource_manager.hpp>
#include <sigmafive/game/component_registry.hpp>
#include <sigmafive/graphics/context_manager.hpp>

namespace sigmafive {
    class engine {
    public:
        engine(int &argc,char **argv);

        system::resource_manager &resource_manager();

        game::component_registry &component_registry();

        graphics::context_manager &graphics_context_manager();
    private:
        system::resource_manager resource_manager_;
        game::component_registry component_registry_;
        graphics::context_manager graphics_context_manager_;
    };
}

#endif //SIGMAFIVE_ENGINE_HPP

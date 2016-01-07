#ifndef SIGMAFIVE_ENGINE_HPP
#define SIGMAFIVE_ENGINE_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>

#include <sigmafive/entity/component_registry.hpp>

#include <sigmafive/system/window_manager.hpp>

#include <sigmafive/graphics/context_manager.hpp>

#include <vector>
#include <boost/dll/shared_library.hpp>

namespace sigmafive {
    class SIGMAFIVE_API engine : public cppbr::meta::object {
    CPPBR_META_CLASS()
    public:
        engine(int &argc, char **argv);

        static boost::filesystem::path default_plugin_path();

        sigmafive::entity::component_registry *component_registry();

        graphics::context_manager *graphics_context_manager();

        system::window_manager *window_manager();

    private:
        //TODO hack
        std::vector<boost::dll::shared_library> plugins_;

        sigmafive::entity::component_registry component_registry_;
        graphics::context_manager graphics_context_manager_;
        system::window_manager window_manager_;
    };
}

#endif //SIGMAFIVE_ENGINE_HPP

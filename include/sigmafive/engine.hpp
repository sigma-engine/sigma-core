#ifndef SIGMAFIVE_ENGINE_HPP
#define SIGMAFIVE_ENGINE_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>

#include <sigmafive/entity/component_cache_factory.hpp>

#include <sigmafive/system/window_manager.hpp>

#include <sigmafive/graphics/context_manager.hpp>

#include <sigmafive/plugin_manager.hpp>

namespace sigmafive {
    class SIGMAFIVE_API engine : public cppbr::meta::object {
    CPPBR_META_CLASS()
    public:
        engine(int &argc, char **argv);

        static boost::filesystem::path default_plugin_path();

        sigmafive::entity::component_cache_factory *component_cache_factory();

        graphics::context_manager *graphics_context_manager();

        system::window_manager *window_manager();

    private:
        plugin_manager plugin_manger;

        sigmafive::entity::component_cache_factory component_cache_factory_;
        graphics::context_manager graphics_context_manager_;
        system::window_manager window_manager_;
    };
}

#endif //SIGMAFIVE_ENGINE_HPP

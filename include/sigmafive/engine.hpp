#ifndef SIGMAFIVE_ENGINE_HPP
#define SIGMAFIVE_ENGINE_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>

#include <sigmafive/graphics/context_manager.hpp>

#include <sigmafive/system/window_manager.hpp>

#include <sigmafive/resource2/loader_manager.hpp>
#include <sigmafive/resource2/package_manager.hpp>
#include <sigmafive/resource2/resource_manager.hpp>

#include <sigmafive/entity/component_cache_factory.hpp>

#include <boost/filesystem/path.hpp>

namespace sigmafive {
    class SIGMAFIVE_API engine : public cppbr::meta::object {
    CPPBR_META_CLASS()
    public:
        engine(int &argc, char **argv);

        static boost::filesystem::path default_plugin_path();

        system::window_manager &window_manager();

        graphics::context_manager &graphics_context_manager();

        resource2::loader_manager &loader_manager();

        resource2::package_manager &package_manager();

        resource2::resource_manager &resource_manager();

        sigmafive::entity::component_cache_factory &component_cache_factory();
    private:
        system::window_manager window_manager_;
        graphics::context_manager graphics_context_manager_;

        resource2::loader_manager loader_manager_;
        resource2::package_manager package_manager_;
        resource2::resource_manager resource_manager_;

        sigmafive::entity::component_cache_factory component_cache_factory_;
    };
}

#endif //SIGMAFIVE_ENGINE_HPP

#include <sigmafive/plugin_manager.hpp>

#include <iostream>
namespace sigmafive {
    plugin_manager::plugin_manager(engine *engine)
        : engine_(engine) {
    }

    plugin_manager::~plugin_manager() {
        for(auto &plugin:plugins) {
            plugin.second.get<void(sigmafive::engine *)>("unregister_plugin")(engine_); // TODO add more error checking!
        }
    }

    bool plugin_manager::load_plugin(boost::filesystem::path plugin_path) {
        if(is_loaded(plugin_path))
            return false;

        auto plugin = std::make_pair(plugin_path,boost::dll::shared_library{plugin_path});
        if(plugin.second.has("register_plugin") && plugin.second.has("unregister_plugin")) {
            plugin.second.get<void(sigmafive::engine *)>("register_plugin")(engine_); // TODO add more error checking!
            plugins.emplace_back(std::move(plugin));
            return true;
        }

        return false;
    }

    bool plugin_manager::is_loaded(boost::filesystem::path plugin_path) const {
        return std::find_if(plugins.begin(),plugins.end(),[plugin_path](const auto &plugin){
            return boost::filesystem::equivalent(plugin.first,plugin_path);
        }) != plugins.end();
    }

    bool plugin_manager::unload_plugin(boost::filesystem::path plugin_path) {
        auto it = std::find_if(plugins.begin(),plugins.end(),[plugin_path](auto &plugin){
            return boost::filesystem::equivalent(plugin.first,plugin_path);
        });
        if(it==plugins.end())
            return false;

        it->second.get<void(sigmafive::engine *)>("unregister_plugin")(engine_); // TODO add more error checking!
        plugins.erase(it);

        return true;
    }
}

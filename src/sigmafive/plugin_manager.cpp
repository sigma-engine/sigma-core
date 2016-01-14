#include <sigmafive/plugin_manager.hpp>

#include <iostream>
#include <boost/range/iterator_range.hpp>

namespace sigmafive {
    plugin_manager::plugin_manager(engine *eng, boost::filesystem::path plugins_path)
        : engine_(eng), plugins_path(plugins_path) {
        //TODO clean this up
        //this is a hack just to get things working
        for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(plugins_path),{})) {
            if (boost::dll::shared_library::suffix() == entry.path().extension()) {
                boost::dll::shared_library library{entry.path()};
                if (library.has("register_plugin")) {
                    plugins.push_back(std::move(library));
                    plugins.back().get<void(engine *)>("register_plugin")(engine_);
                }
                else {
                    std::cout << "warning: shared library in plugins folder that does not export a plugin." << std::endl;
                }
            }
        }
    }

    plugin_manager::~plugin_manager() {
        for(auto &plugin: plugins) {
            try {
                plugin.get<void(engine *)>("unregister_plugin")(engine_);
            } catch(...) {

            }
        }
    }
}


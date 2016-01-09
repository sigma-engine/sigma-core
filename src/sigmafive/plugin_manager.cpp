#include <sigmafive/plugin_manager.hpp>

#include <iostream>
#include <boost/range/iterator_range.hpp>

namespace sigmafive {
    plugin_manager::plugin_manager(boost::filesystem::path plugins_path) : plugins_path(plugins_path) {}
    /*void plugin_manager::load_plugins(boost::filesystem::path plugins_path, engine *e) {
        //TODO clean this up
        //this is a hack just to get things working
        for (auto &entry : boost::make_iterator_range(boost::filesystem::directory_iterator(plugins_path),{})) {
            if (boost::dll::shared_library::suffix() == entry.path().extension()) {
                boost::dll::shared_library library{entry.path()};
                if (library.has("register_plugin")) {
                    plugins_.push_back(std::move(library));
                    plugins_.back().get<void(engine *)>("register_plugin")(e);
                }
                else {
                    std::cout << "warning: shared library in plugins folder that does not export a plugin." << std::endl;
                }
            }
        }
    }*/
}


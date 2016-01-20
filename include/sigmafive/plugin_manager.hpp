#pragma once

#include <sigmafive/config.hpp>

#include <unordered_map>
#include <boost/filesystem/path.hpp>
#include <boost/dll/shared_library.hpp>


namespace sigmafive {
    class engine;

    class SIGMAFIVE_API plugin_manager {
    public:
        plugin_manager(sigmafive::engine *engine);

        ~plugin_manager();

        bool load_plugin(boost::filesystem::path plugin_path);

        bool is_loaded(boost::filesystem::path plugin_path) const;

        bool unload_plugin(boost::filesystem::path plugin_path);
    private:
        sigmafive::engine *engine_;
        std::list<std::pair<boost::filesystem::path,boost::dll::shared_library>> plugins;
    };
}


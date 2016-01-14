#ifndef SIGMA_FIVE_ENGINE_PLUGIN_MANAGER_HPP
#define SIGMA_FIVE_ENGINE_PLUGIN_MANAGER_HPP

#include <sigmafive/config.hpp>

#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/dll/shared_library.hpp>

namespace sigmafive {
	class engine;

	class SIGMAFIVE_API plugin_manager {
	public:
		plugin_manager(engine *eng,boost::filesystem::path plugins_path);

		~plugin_manager();
	private:
		engine *engine_;
		boost::filesystem::path plugins_path;
		std::vector<boost::dll::shared_library> plugins;
	};
}

#endif // SIGMA_FIVE_ENGINE_PLUGIN_MANAGER_HPP

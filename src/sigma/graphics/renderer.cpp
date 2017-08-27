#include <sigma/graphics/renderer.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>

#include <fstream>

namespace sigma {
namespace graphics {
    renderer::renderer(glm::ivec2 size)
    {
        // TODO this is a hack
        auto cache_path = boost::filesystem::current_path() / "data";
        if (!boost::filesystem::exists(cache_path))
            cache_path = boost::filesystem::current_path() / ".." / "data";

        auto settings_file = cache_path / "graphics" / "settings";
        if (boost::filesystem::exists(settings_file)) {
            std::ifstream file{ settings_file.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            ia >> settings_;
        }
    }

    renderer::~renderer() {}
}
}

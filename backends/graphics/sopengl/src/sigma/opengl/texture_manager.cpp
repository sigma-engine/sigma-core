#include <sigma/opengl/texture_manager.hpp>

namespace sigma {
namespace opengl {
    texture_manager::texture_manager(resource::cache<graphics::texture> &cache)
        : resource::resource_manager<graphics::texture, opengl::texture>(cache)
    {
    }

    std::shared_ptr<opengl::texture> texture_manager::create(const graphics::texture& cpu_texture)
    {
        // TODO support more than just rgba interal formats
        return std::make_shared<opengl::texture>(internal_format::RGBA, cpu_texture.size(), cpu_texture.data());
    }
}
}

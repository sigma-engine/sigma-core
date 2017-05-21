#include <sigma/graphics/material.hpp>

namespace sigma {
namespace graphics {
    material::material(texture_manager& textures, cubemap_manager& cubemaps, shader_manager& shaders, const material_data& data)
        : shader_technique(textures, cubemaps, shaders, data)
    {
        transparent_ = data.transparent;
    }

    bool material::is_transparent() const
    {
        return transparent_;
    }
}
}

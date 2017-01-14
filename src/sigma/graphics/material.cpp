#include <sigma/graphics/material.hpp>

namespace sigma {
namespace graphics {
    material::material(const material_data& data)
        : shader_technique(data)
    {
        transparent_ = data.transparent;
    }

    bool material::is_transparent() const
    {
        return transparent_;	
    }
}
}

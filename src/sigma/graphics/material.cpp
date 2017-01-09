#include <sigma/graphics/material.hpp>

namespace sigma {
namespace graphics {
    material::material(const material_data &data)
        : data_(data)
    {
    }

    bool material::is_transparent() const
    {
        return data_.transparent;
    }
}
}

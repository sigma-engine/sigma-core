#include <sigma/graphics/material.hpp>

namespace sigma {
namespace graphics {
    material::material(material_data data)
        : data_(std::move(data))
    {
    }

    bool material::is_transparent() const
    {
        return data_.transparent;
    }
}
}

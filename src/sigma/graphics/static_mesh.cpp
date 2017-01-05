#include <sigma/graphics/static_mesh.hpp>

namespace sigma {
namespace graphics {
    resource::handle<graphics::material> static_mesh::material()
    {
        return material_;
    }

    void static_mesh::set_material(resource::handle<graphics::material> mat)
    {
        if (material_ != mat)
            material_ = mat;
    }
}
}

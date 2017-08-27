#include <sigma/graphics/opengl/material.hpp>

namespace sigma {
namespace opengl {
    material::material(technique_manager& technique_mgr, const graphics::material& data)
        : data(data)
    {
        TECHNIQUE_PTR(technique_mgr, data.technique_id)->gather_uniform_locations(data);
    }
}
}

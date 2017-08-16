#include <sigma/graphics/opengl/material.hpp>

namespace sigma {
namespace opengl {
    material::material(technique_manager& technique_mgr, const graphics::material& data)
        : technique(technique_mgr.get(data.technique_id))
        , data(data)
    {
        TECHNIQUE_PTR(technique_mgr, technique)->gather_uniform_locations(data);
    }
}
}

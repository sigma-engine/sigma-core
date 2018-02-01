#include <sigma/graphics/opengl/post_process_effect.hpp>

namespace sigma {
namespace opengl {
    post_process_effect::post_process_effect(technique_manager& technique_mgr, const graphics::post_process_effect& data)
        : data(data)
    {
        TECHNIQUE_PTR(technique_mgr, data.technique_id)->gather_uniform_locations(data);
    }
}
}

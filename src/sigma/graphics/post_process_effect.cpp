#include <sigma/graphics/post_process_effect.hpp>

namespace sigma {
namespace graphics {
    post_process_effect::post_process_effect()
    {
        shaders[shader_type::vertex] = resource::identifier{ "vertex://fullscreen_quad" };
    }
}
}

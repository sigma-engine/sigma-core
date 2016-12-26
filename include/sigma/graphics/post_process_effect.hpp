#ifndef SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
#define SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/shader_technique.hpp>

namespace sigma {
namespace graphics {
    class SIGMA_API post_process_effect : public shader_technique {
    public:
        post_process_effect();

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

    private:
        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };

    using post_process_effect_cache = shader_technique_cache<post_process_effect>;
}
}

#endif //SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

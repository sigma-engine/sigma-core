#ifndef SIGMA_ENGINE_OPENGL_POST_PROCESS_EFFECT_MANAGER_HPP
#define SIGMA_ENGINE_OPENGL_POST_PROCESS_EFFECT_MANAGER_HPP

#include <sigma/opengl/post_process_effect.hpp>

#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/resource/resource_manager.hpp>

namespace sigma {
namespace opengl {
    class texture_manager;
    class shader_manager;
    class static_mesh_manager;
    class post_process_effect_manager : public resource::resource_manager<graphics::post_process_effect, opengl::post_process_effect> {
    public:
        post_process_effect_manager(resource::resource_cache<graphics::post_process_effect>& cache, texture_manager& textures, shader_manager& shaders, static_mesh_manager& meshes);

        virtual std::shared_ptr<opengl::post_process_effect> create(const graphics::post_process_effect& cpu_effect) override;

    private:
        post_process_effect_manager(const post_process_effect_manager&) = delete;
        post_process_effect_manager& operator=(const post_process_effect_manager&) = delete;

        texture_manager& textures_;
        shader_manager& shaders_;
        static_mesh_manager& meshes_;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_POST_PROCESS_EFFECT_MANAGER_HPP

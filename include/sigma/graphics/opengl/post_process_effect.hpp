#ifndef SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
#define SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/post_process_effect.hpp>

#include <sigma/graphics/opengl/shader_technique.hpp>
#include <sigma/graphics/static_mesh.hpp>

#include <glad/glad.h>

#define EFFECT_PTR(effect_mgr, x) effect_mgr.acquire(x)

namespace sigma {
namespace opengl {
    class cubemap_manager;
    class shader_manager;
    class static_mesh_manager;
    class texture_manager;

    class post_process_effect : public shader_technique<graphics::post_process_effect> {
    public:
        post_process_effect(static_mesh_manager& static_meshes, const graphics::post_process_effect& data);

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        void link(texture_manager& texture_mgr, cubemap_manager& cubemap_mgr, shader_manager& shader_mgr);

        void bind(opengl::texture_manager& texture_mgr, opengl::cubemap_manager& cubemap_mgr) const;

        void apply(static_mesh_manager& static_mesh_mgr) const;

    private:
        GLint in_position_location_ = -1;
        GLint in_diffuse_location_ = -1;
        GLint in_normal_location_ = -1;
        GLint in_depth_stencil_location_ = -1;
        GLint in_shadow_map0_location_ = -1;
        GLint in_shadow_map1_location_ = -1;
        GLint in_shadow_map2_location_ = -1;

        resource::handle<graphics::static_mesh> mesh_;

        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };

    class post_process_effect_manager {
    public:
        // TODO remove the use of unique_ptr

        post_process_effect_manager(texture_manager& textures, cubemap_manager& cubemaps, shader_manager& shaders, static_mesh_manager& static_meshes, resource::cache<graphics::post_process_effect>& post_process_effect_cache)
            : textures_(textures)
            , cubemaps_(cubemaps)
            , shaders_(shaders)
            , static_meshes_(static_meshes)
            , post_process_effect_cache_(post_process_effect_cache)
        {
        }

        resource::handle<graphics::post_process_effect> get(boost::filesystem::path id)
        {
            return post_process_effect_cache_.get(id);
        }

        opengl::post_process_effect* acquire(resource::handle<graphics::post_process_effect> hndl)
        {
            // TODO not thread safe
            if (hndl.index >= effects_.size())
                effects_.resize(hndl.index + 1);

            if (effects_[hndl.index] == nullptr) {
                effects_[hndl.index] = std::make_unique<post_process_effect>(static_meshes_, *post_process_effect_cache_.acquire(hndl));
                effects_[hndl.index]->link(textures_, cubemaps_, shaders_);
            }

            return effects_.at(hndl.index).get();
        }

    private:
        texture_manager& textures_;
        cubemap_manager& cubemaps_;
        shader_manager& shaders_;
        static_mesh_manager& static_meshes_;
        resource::cache<graphics::post_process_effect>& post_process_effect_cache_;
        std::vector<std::unique_ptr<post_process_effect>> effects_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP

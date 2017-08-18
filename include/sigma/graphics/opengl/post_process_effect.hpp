#ifndef SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP
#define SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/post_process_effect.hpp>

#include <sigma/graphics/opengl/technique.hpp>
#include <sigma/graphics/static_mesh.hpp>

#define EFFECT_PTR(effect_mgr, x) effect_mgr.acquire(x)

namespace sigma {
namespace opengl {
    class cubemap_manager;
    class shader_manager;
    class static_mesh_manager;
    class texture_manager;

    class post_process_effect {
    public:
        post_process_effect(technique_manager& technique_mgr, static_mesh_manager& static_meshes, const graphics::post_process_effect& data);

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        resource::handle<graphics::technique> technique;

        resource::handle<graphics::static_mesh> mesh;

        const graphics::post_process_effect& data;

    private:
        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };

    class post_process_effect_manager {
    public:
        // TODO remove the use of unique_ptr

        post_process_effect_manager(technique_manager& techniques, static_mesh_manager& static_meshes, resource::cache<graphics::post_process_effect>& post_process_effect_cache)
            : techniques_(techniques)
            , static_meshes_(static_meshes)
            , post_process_effect_cache_(post_process_effect_cache)
        {
        }

        resource::handle<graphics::post_process_effect> get(const boost::filesystem::path& id)
        {
            return post_process_effect_cache_.get(id);
        }

        opengl::post_process_effect* acquire(const resource::handle<graphics::post_process_effect>& hndl)
        {
            // TODO not thread safe
            if (hndl.index >= post_process_effects_.size())
                post_process_effects_.resize(hndl.index + 1);

            if (post_process_effects_[hndl.index] == nullptr)
                post_process_effects_[hndl.index] = std::make_unique<post_process_effect>(techniques_, static_meshes_, *post_process_effect_cache_.acquire(hndl));

            return post_process_effects_.at(hndl.index).get();
        }

    private:
        technique_manager& techniques_;
        static_mesh_manager& static_meshes_;
        resource::cache<graphics::post_process_effect>& post_process_effect_cache_;
        std::vector<std::unique_ptr<post_process_effect>> post_process_effects_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_POST_PROCESS_EFFECT_HPP

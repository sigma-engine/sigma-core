#ifndef SIGMA_GRAPHICS_OPENGL_CUBEMAP_HPP
#define SIGMA_GRAPHICS_OPENGL_CUBEMAP_HPP

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/manager.hpp>

#include <glad/glad.h>

#define CUBEMAP_PTR(cubemap_mgr, x) cubemap_mgr.acquire(x)

namespace sigma {
namespace opengl {
    class cubemap {
    public:
        cubemap(resource::cache<graphics::texture>& texture_cache, const graphics::cubemap& data);

        cubemap(cubemap&&) = default;

        cubemap& operator=(cubemap&&) = default;

        ~cubemap();

        GLuint get_object() const;

        void bind() const;

    private:
        cubemap(const cubemap&) = delete;
        cubemap& operator=(const cubemap&) = delete;

        GLuint object_ = 0;
    };

    class cubemap_manager {
    public:
        // TODO remove the use of unique_ptr

        cubemap_manager(resource::cache<graphics::texture>& texture_cache, resource::cache<graphics::cubemap>& cubemap_cache)
            : texture_cache_(texture_cache)
            , cubemap_cache_(cubemap_cache)
        {
        }

        resource::handle<graphics::cubemap> get(resource::identifier id)
        {
            return cubemap_cache_.get(id);
        }

        opengl::cubemap* acquire(resource::handle<graphics::cubemap> hndl)
        {
            // TODO not thread safe
            if (hndl.index >= cubemaps_.size())
                cubemaps_.resize(hndl.index + 1);

            if (cubemaps_[hndl.index] == nullptr)
                cubemaps_[hndl.index] = std::make_unique<cubemap>(texture_cache_, *cubemap_cache_.acquire(hndl));

            return cubemaps_.at(hndl.index).get();
        }

    private:
        resource::cache<graphics::texture>& texture_cache_;
        resource::cache<graphics::cubemap>& cubemap_cache_;
        std::vector<std::unique_ptr<cubemap>> cubemaps_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_CUBEMAP_HPP

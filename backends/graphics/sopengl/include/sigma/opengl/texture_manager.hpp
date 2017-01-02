#ifndef SIGMA_ENGINE_OPENGL_TEXTURE_MANAGER_HPP
#define SIGMA_ENGINE_OPENGL_TEXTURE_MANAGER_HPP

#include <sigma/opengl/texture.hpp>
#include <sigma/opengl/resource_manager.hpp>

#include <sigma/graphics/texture.hpp>

namespace sigma {
namespace opengl {
    class texture_manager : public resource::resource_manager<graphics::texture, opengl::texture> {
    public:
        texture_manager(resource::cache<graphics::texture>& cache);

        virtual std::shared_ptr<opengl::texture> create(const graphics::texture& cpu_texture) override;

    private:
        texture_manager(const texture_manager&) = delete;
        texture_manager& operator=(const texture_manager&) = delete;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_TEXTURE_MANAGER_HPP

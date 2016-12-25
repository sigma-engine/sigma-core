#ifndef SIGMA_ENGINE_OPENGL_TEXTURE_MANAGER_HPP
#define SIGMA_ENGINE_OPENGL_TEXTURE_MANAGER_HPP

#include <sigma/opengl/texture.hpp>

#include <sigma/graphics/texture.hpp>
#include <sigma/resource/resource_manager.hpp>

namespace sigma {
namespace opengl {
    class texture_manager : public resource::resource_manager<graphics::texture, opengl::texture> {
    public:
		texture_manager(resource::resource_cache<graphics::texture>& cache);

		virtual std::shared_ptr<opengl::texture> create(const graphics::texture& cpu_texture) override;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_TEXTURE_MANAGER_HPP

#ifndef SIGMA_ENGINE_OPENGL_SHADER_MANAGER_HPP
#define SIGMA_ENGINE_OPENGL_SHADER_MANAGER_HPP

#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/resource_manager.hpp>

#include <sigma/graphics/shader.hpp>

namespace sigma {
namespace opengl {
    class shader_manager : public resource::resource_manager<graphics::shader, opengl::shader> {
    public:
        shader_manager(resource::cache<graphics::shader>& cache);

        virtual std::shared_ptr<opengl::shader> create(const graphics::shader& cpu_shader) override;

    private:
        shader_manager(const shader_manager&) = delete;
        shader_manager& operator=(const shader_manager&) = delete;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_SHADER_MANAGER_HPP

#ifndef SIGMA_ENGINE_SHADER_MANAGER_HPP
#define SIGMA_ENGINE_SHADER_MANAGER_HPP

#include <sigma/graphics/shader.hpp>

#include <sigma/config.hpp>
#include <sigma/resource/cache.hpp>

#include <glad/glad.h>

#include <string>

namespace sigma {
namespace opengl {
    GLenum convert(graphics::shader_type type);

    class shader_manager {
    public:
        shader_manager(resource::cache<graphics::shader>& shader_cache);

        shader_manager(shader_manager&&) = default;

        shader_manager& operator=(shader_manager&&) = default;

        ~shader_manager();

        std::pair<graphics::shader*, GLuint> acquire(const resource::handle<graphics::shader>& hndl);

    private:
        shader_manager(const shader_manager&) = delete;
        shader_manager& operator=(const shader_manager&) = delete;

        resource::cache<graphics::shader>& shader_cache_;
        std::vector<GLuint> shaders_;
    };
}
}

#endif // SIGMA_ENGINE_SHADER_MANAGER_HPP

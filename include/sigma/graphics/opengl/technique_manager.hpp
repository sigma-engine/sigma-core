#ifndef SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_MANAGER_HPP
#define SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_MANAGER_HPP

#include <sigma/graphics/technique.hpp>

#include <sigma/config.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    class shader_manager;

    class technique_manager {
    public:
        technique_manager(resource::cache<graphics::technique>& technique_cache, shader_manager& shaders);

        technique_manager(technique_manager&&) = default;

        technique_manager& operator=(technique_manager&&) = default;

        ~technique_manager();

        std::pair<graphics::technique*, GLuint> acquire(const resource::handle<graphics::technique>& hndl);

    private:
        technique_manager(const technique_manager&) = delete;
        technique_manager& operator=(const technique_manager&) = delete;

        resource::cache<graphics::technique>& technique_cache_;
        shader_manager& shaders_;
        std::vector<GLuint> programs_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_SHADER_TECHNIQUE_MANAGER_HPP

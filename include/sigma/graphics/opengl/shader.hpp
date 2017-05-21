#ifndef SIGMA_ENGINE_SHADER_HPP
#define SIGMA_ENGINE_SHADER_HPP

#include <sigma/graphics/shader.hpp>
#include <sigma/resource/manager.hpp>

#include <glad/glad.h>

#include <string>

#define SHADER_PTR(shader_mgr, x) shader_mgr.acquire(x)

namespace sigma {
namespace opengl {
    enum class shader_type : GLenum {
        //COMPUTE_SHADER = GL_COMPUTE_SHADER,
        VERTEX_SHADER = GL_VERTEX_SHADER,
        TESS_CONTROL_SHADER = GL_TESS_CONTROL_SHADER,
        TESS_EVALUATION_SHADER = GL_TESS_EVALUATION_SHADER,
        GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
        FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
        NONE_SHADER
    };

    class shader {
    public:
        shader(shader_type type, std::string source);

        shader(graphics::shader data);

        shader(shader&&) = default;

        shader& operator=(shader&&) = default;

        virtual ~shader();

        GLuint get_object() const;

    private:
        shader(const shader&) = delete;
        shader& operator=(const shader&) = delete;

        GLuint object_ = 0;
    };

    class shader_manager {
    public:
        // TODO remove the use of unique_ptr

        shader_manager(resource::cache<graphics::shader>& shader_cache)
            : shader_cache_(shader_cache)
        {
        }

        resource::handle<graphics::shader> get(resource::identifier id)
        {
            return shader_cache_.get(id);
        }

        opengl::shader* acquire(resource::handle<graphics::shader> hndl)
        {
            // TODO not thread safe
            if (hndl.index >= shaders_.size())
                shaders_.resize(hndl.index + 1);

            if (shaders_[hndl.index] == nullptr)
                shaders_[hndl.index] = std::make_unique<shader>(*shader_cache_.acquire(hndl));

            return shaders_.at(hndl.index).get();
        }

    private:
        resource::cache<graphics::shader>& shader_cache_;
        std::vector<std::unique_ptr<shader>> shaders_;
    };
}
}

#endif // SIGMA_ENGINE_SHADER_HPP

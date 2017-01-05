#ifndef SIGMA_ENGINE_OPENGL_RENDER_UNIFORMS_HPP
#define SIGMA_ENGINE_OPENGL_RENDER_UNIFORMS_HPP

#include <glm/matrix.hpp>

namespace sigma {
namespace opengl {
    struct standard_uniforms {
        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;
        glm::vec2 view_port_size;
        float time;
        float z_near;
        float z_far;
    };

    struct render_matrices {
        glm::mat4 model_matrix;
        glm::mat4 model_view_matrix;
        glm::mat3 normal_matrix;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_RENDER_UNIFORMS_HPP

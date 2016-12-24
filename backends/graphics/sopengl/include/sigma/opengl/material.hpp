#ifndef SIGMA_FIVE_OPENGL_MATERIAL_HPP
#define SIGMA_FIVE_OPENGL_MATERIAL_HPP

#include <sigma/opengl/config.hpp>

#include <boost/filesystem/path.hpp>
#include <chrono>
#include <glm/mat4x4.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/opengl/gl_core_4_0.h>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/texture.hpp>
#include <unordered_map>
#include <vector>

namespace sigma {
namespace opengl {
    struct material {
        static constexpr const char* PROJECTION_MATRIX_NAME = "projectionMatrix";
        static constexpr const char* VIEW_MATRIX_NAME = "viewMatrix";
        static constexpr const char* MODEL_MATRIX_NAME = "modelMatrix";
        static constexpr const char* MODEL_VIEW_MATRIX_NAME = "modelViewMatrix";
        static constexpr const char* NORMAL_MATRIX_NAME = "normalMatrix";
        static constexpr const char* TIME_NAME = "time";

        GLuint object = 0;
        // TOOD move these to uniform buffers.
        GLint projection_matrix_location = -1;
        GLint view_matrix_location = -1;
        GLint model_matrix_location = -1;
        GLint model_view_matrix_location = -1;
        GLint normal_matrix_location = -1;
        GLint time_location = -1;
        std::vector<GLint> texture_locations;
    };
}
}

#endif // SIGMA_FIVE_OPENGL_MATERIAL_HPP

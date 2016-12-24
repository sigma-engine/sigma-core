#ifndef SIGMA_FIVE_OPENGL_TEXTURE_HPP
#define SIGMA_FIVE_OPENGL_TEXTURE_HPP

#include <sigma/opengl/config.hpp>

#include <sigma/graphics/texture.hpp>

#include <chrono>
#include <unordered_map>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <glm/mat4x4.hpp>

#include <sigma/opengl/gl_core_4_0.h>
#include <sigma/opengl/shader.hpp>

namespace sigma {
namespace opengl {
    struct texture {
        GLuint object = 0;
    };
}
}

#endif // SIGMA_FIVE_OPENGL_TEXTURE_HPP

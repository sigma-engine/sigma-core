#ifndef SIGMA_FIVE_OPENGL_TEXTURE_HPP
#define SIGMA_FIVE_OPENGL_TEXTURE_HPP

#include <sigma/opengl/config.hpp>

#include <boost/filesystem/path.hpp>
#include <chrono>
#include <glm/mat4x4.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/opengl/gl_core_4_0.h>
#include <sigma/opengl/shader.hpp>
#include <unordered_map>
#include <vector>

namespace sigma {
namespace opengl {
    struct texture {
        GLuint object = 0;
    };
}
}

#endif // SIGMA_FIVE_OPENGL_TEXTURE_HPP

#ifndef SIGMA_FIVE_OPENGL_SHADER_HPP
#define SIGMA_FIVE_OPENGL_SHADER_HPP

#include <sigma/opengl/config.hpp>

#include <boost/filesystem/path.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/opengl/gl_core_4_0.h>

namespace sigma {
namespace opengl {

    struct shader {
        GLuint object = 0;
    };
}
}

#endif // SIGMA_FIVE_OPENGL_SHADER_HPP

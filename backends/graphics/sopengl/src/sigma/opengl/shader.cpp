#include <sigma/opengl/shader.hpp>

#include <fstream>
#include <iostream>
#include <string>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {


    /*shader_cache::~shader_cache()
    {
        for (auto& shader : shaders_)
          glDeleteShader(shader.object);
    }

    GLuint shader_cache::get_object(resource::identifier shader_id)
    {
        if (!is_cached(shader_id))
            return 0;
        return shaders_[resource_map_[shader_id]].object;
    }

    void shader_cache::update()
    {
        std::for_each(std::begin(shaders_) + static_cast<difference_type>(dirty_), std::end(shaders_), [](opengl::shader& shader) {
            if (shader.object == 0)
                GL_CHECK(shader.object = glCreateShader(convert(shader.type)));
            const char* source = shader.source.c_str();
            GL_CHECK(glShaderSource(shader.object, 1, &source, nullptr));
            GL_CHECK(glCompileShader(shader.object));
        });

        // TODO free any shaders that have a reference count of zero

        dirty_ = shaders_.size();
    }*/
}
}

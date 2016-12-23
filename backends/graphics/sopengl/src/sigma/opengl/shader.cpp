#include <sigma/opengl/shader.hpp>

#include <fstream>
#include <iostream>
#include <string>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    GLenum convert(graphics::shader_type type)
    {
        switch (type) {
        case graphics::shader_type::vertex:
            return GL_VERTEX_SHADER;
        case graphics::shader_type::fragment:
            return GL_FRAGMENT_SHADER;
        case graphics::shader_type::none:
            return GL_NONE;
        }
        return GL_NONE;
    }

    shader_cache::shader_cache()
        : cache_directory_(boost::filesystem::current_path() / ".." / "data" / "opengl")
        , dirty_(shaders_.size())
    {
    }

    shader_cache::~shader_cache()
    {
        for (auto& shader : shaders_)
            glDeleteShader(shader.object);
    }

    bool shader_cache::is_cached(resource::identifier shader_id) const
    {
        return resource_map_.count(shader_id) > 0;
    }

    bool shader_cache::increment_reference(resource::identifier shader_id)
    {
        if (is_cached(shader_id)) {
            shaders_[resource_map_[shader_id]].reference_count++;
            return true;
        }

        shader new_shader;
        try {
            auto shader_path = cache_directory_ / std::to_string(shader_id.value());
            std::ifstream file{ shader_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            ia >> new_shader;
        } catch (boost::archive::archive_exception& ex) {
            std::cout << "shader: " << shader_id << " " << ex.what() << std::endl;
            return false;
        } catch (std::exception& ex) {
            std::cout << "shader: " << shader_id << " " << ex.what() << std::endl;
            return false;
        } catch (...) { // TODO check for correct errors here
            std::cout << "shader: unknown exception " << std::endl;
            return false;
        }

        auto index = shaders_.size();
        if (dirty_ >= index)
            dirty_ = index;
        resource_map_[shader_id] = index;
        new_shader.reference_count = 1;
        shaders_.push_back(std::move(new_shader));

        return true;
    }

    bool shader_cache::decrement_reference(resource::identifier shader_id)
    {
        if (!is_cached(shader_id))
            return false;
        auto index = resource_map_[shader_id];
        if (shaders_[index].reference_count > 0)
            shaders_[index].reference_count--;
        return shaders_[index].reference_count == 0;
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
    }
}
}

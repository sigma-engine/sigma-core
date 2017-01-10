#include <sigma/opengl/shader_technique.hpp>

#include <sigma/opengl/geometry_buffer.hpp>
#include <sigma/opengl/render_uniforms.hpp>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/texture.hpp>
#include <sigma/opengl/util.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <cassert>
#include <iostream>

namespace sigma {
namespace opengl {

    /*void shader_technique::set_texture(std::string name, resource::handle<graphics::texture> txt)
    {
        auto it = texture_map_.find(name);
        if (it == texture_map_.end()) {
            auto ind = get_uniform_location(name.c_str());
            // TODO maybe warn about the texture not being used?
            //assert(ind >= 0 && "Program does not have this texture!");
            if (ind >= 0) {
                texture_map_[name] = textures_.size();
                textures_.push_back(std::make_pair(ind, txt));
            }
            return;
        }
        textures_[texture_map_[name]].second = txt;
    }*/
}
}

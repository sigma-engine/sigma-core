#include <sigma/opengl/texture.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sigma/opengl/util.hpp>
#include <string>

namespace sigma {
namespace opengl {
    /*texture_cache::~texture_cache()
    {
        for (const auto& texture : textures_)
            glDeleteTextures(1, &texture.object);
    }

    void texture_cache::bind(resource::identifier texture_id) const
    {
        if (!is_cached(texture_id))
            return; // TODO apply default instead?
        auto index = resource_map_.at(texture_id);
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, textures_[index].object));
    }

    void texture_cache::update()
    {
        std::for_each(std::begin(textures_) + static_cast<difference_type>(dirty_), std::end(textures_), [this](opengl::texture& texture) {
            if (texture.object == 0)
                GL_CHECK(glGenTextures(1, &texture.object));

            GL_CHECK(glBindTexture(GL_TEXTURE_2D, texture.object));
            GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

            GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width(), texture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data().data()));
        });
        dirty_ = textures_.size();
    }*/
}
}

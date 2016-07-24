#include <sigma/opengl/texture.hpp>

#include <fstream>
#include <iostream>
#include <string>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>

#include <glm/gtc/type_ptr.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    texture_cache::texture_cache()
        : cache_directory_(boost::filesystem::current_path() / ".." / "data" / "opengl")
        , dirty_(textures_.size())
    {
    }

    texture_cache::~texture_cache()
    {
        for (const auto& texture : textures_)
            glDeleteTextures(1, &texture.object);

        // TODO free all textures
    }

    bool texture_cache::is_cached(resource::identifier texture_id) const
    {
        return resource_map_.count(texture_id) > 0;
    }

    bool texture_cache::increment_reference(resource::identifier texture_id)
    {
        if (is_cached(texture_id)) {
            textures_[resource_map_[texture_id]].reference_count++;
            return true;
        }

        texture new_texture;
        try {
            auto texture_path = cache_directory_ / std::to_string(texture_id.value());
            std::ifstream file{ texture_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            ia >> new_texture;
        } catch (boost::archive::archive_exception& ex) {
            std::cout << "texture: " << texture_id << " " << ex.what() << std::endl;
            return false;
        } catch (std::exception& ex) {
            std::cout << "texture: " << texture_id << " " << ex.what() << std::endl;
            return false;
        } catch (...) { // TODO check for correct errors here
            std::cout << "texture: unknown exception " << std::endl;
            return false;
        }

        auto index = textures_.size();
        if (dirty_ >= index)
            dirty_ = index;

        resource_map_[texture_id] = index;
        new_texture.reference_count = 1;
        textures_.push_back(std::move(new_texture));

        return true;
    }

    bool texture_cache::decrement_reference(resource::identifier texture_id)
    {
        if (!is_cached(texture_id))
            return false;
        auto index = resource_map_[texture_id];
        if (textures_[index].reference_count > 0)
            textures_[index].reference_count--;
        return textures_[index].reference_count == 0;
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
    }
}
}

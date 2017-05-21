#include <sigma/graphics/opengl/material.hpp>

#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

namespace sigma {
namespace opengl {
    material::material(texture_manager& textures, cubemap_manager& cubemaps, shader_manager& shaders, const graphics::material_data& data)
        : shader_technique<graphics::material>(textures, cubemaps, shaders, data)
    {
    }

    void material::link(opengl::shader_manager& shader_mgr)
    {
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::NORMAL_METALNESS_LOCATION, geometry_buffer::NORMAL_METALNESS_OUTPUT_NAME));
        shader_technique::link(shader_mgr);
    }

    material_manager::material_manager(boost::filesystem::path cache_directory, opengl::shader_manager& shaders, opengl::texture_manager& textures, opengl::cubemap_manager& cubemaps)
        : graphics::material_manager(cache_directory)
        , shaders_(shaders)
        , textures_(textures)
        , cubemaps_(cubemaps)
    {
    }

    std::unique_ptr<graphics::material> material_manager::create(graphics::material_data data)
    {
        auto effect = std::make_unique<opengl::material>(textures_, cubemaps_, shaders_, std::move(data));
        effect->link(shaders_);
        return std::move(effect);
    }
}
}

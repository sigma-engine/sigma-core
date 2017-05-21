#include <sigma/graphics/opengl/post_process_effect.hpp>

#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/static_mesh.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glad/glad.h>

#include <boost/filesystem/path.hpp>

namespace sigma {
namespace opengl {
    post_process_effect::post_process_effect(texture_manager& textures, cubemap_manager& cubemaps, shader_manager& shaders, static_mesh_manager& static_meshes, const graphics::post_process_effect_data& data)
        : shader_technique<graphics::post_process_effect>(textures, cubemaps, shaders, static_meshes, data)
    {
    }

    void post_process_effect::link(opengl::shader_manager& shader_mgr)
    {
        shader_technique::link(shader_mgr);
        GL_CHECK(in_diffuse_location_ = glGetUniformLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_INPUT_NAME));
        GL_CHECK(in_normal_location_ = glGetUniformLocation(object_, geometry_buffer::NORMAL_METALNESS_INPUT_NAME));
        GL_CHECK(in_depth_stencil_location_ = glGetUniformLocation(object_, geometry_buffer::DEPTH_STENCIL_INPUT_NAME));
        GL_CHECK(in_shadow_map_location_ = glGetUniformLocation(object_, geometry_buffer::SHADOW_MAP_INPUT_NAME));
    }

    void post_process_effect::bind(const opengl::texture_manager& texture_mgr, const opengl::cubemap_manager& cubemap_mgr) const
    {
        shader_technique::bind(texture_mgr, cubemap_mgr, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
        GL_CHECK(glUniform1i(in_diffuse_location_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(in_normal_location_, geometry_buffer::NORMAL_METALNESS_LOCATION));
        GL_CHECK(glUniform1i(in_depth_stencil_location_, geometry_buffer::DEPTH_STENCIL_LOCATION));
        GL_CHECK(glUniform1i(in_shadow_map_location_, geometry_buffer::SHADOW_MAP_LOCATION));
    }

    void post_process_effect::apply(const static_mesh_manager& static_mesh_mgr) const
    {
        STATIC_MESH_PTR(static_mesh_mgr, mesh_)->render();
    }

    post_process_effect_manager::post_process_effect_manager(boost::filesystem::path cache_directory, opengl::shader_manager& shaders, opengl::texture_manager& textures, opengl::cubemap_manager& cubemaps, opengl::static_mesh_manager& meshes)
        : graphics::post_process_effect_manager(cache_directory)
        , shaders_(shaders)
        , textures_(textures)
        , cubemaps_(cubemaps)
        , meshes_(meshes)
    {
    }

    std::unique_ptr<graphics::post_process_effect> post_process_effect_manager::create(graphics::post_process_effect_data data)
    {
        auto effect = std::make_unique<opengl::post_process_effect>(textures_, cubemaps_, shaders_, meshes_, std::move(data));
        effect->link(shaders_);
        return std::move(effect);
    }
}
}

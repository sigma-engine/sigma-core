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
    post_process_effect::post_process_effect(static_mesh_manager& static_meshes, const graphics::post_process_effect& data)
        : shader_technique<graphics::post_process_effect>(data)
    {
        mesh_ = static_meshes.get(data.mesh);
    }

    void post_process_effect::link(texture_manager& texture_mgr, cubemap_manager& cubemap_mgr, shader_manager& shader_mgr)
    {
        shader_technique::link(texture_mgr, cubemap_mgr, shader_mgr);
        GL_CHECK(in_diffuse_location_ = glGetUniformLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_INPUT_NAME));
        GL_CHECK(in_normal_location_ = glGetUniformLocation(object_, geometry_buffer::NORMAL_METALNESS_INPUT_NAME));
        GL_CHECK(in_depth_stencil_location_ = glGetUniformLocation(object_, geometry_buffer::DEPTH_STENCIL_INPUT_NAME));
        GL_CHECK(in_shadow_map0_location_ = glGetUniformLocation(object_, geometry_buffer::SHADOW_MAP0_INPUT_NAME));
        GL_CHECK(in_shadow_map1_location_ = glGetUniformLocation(object_, geometry_buffer::SHADOW_MAP1_INPUT_NAME));
        GL_CHECK(in_shadow_map2_location_ = glGetUniformLocation(object_, geometry_buffer::SHADOW_MAP2_INPUT_NAME));
    }

    void post_process_effect::bind(opengl::texture_manager& texture_mgr, opengl::cubemap_manager& cubemap_mgr) const
    {
        shader_technique::bind(texture_mgr, cubemap_mgr, geometry_buffer::NEXT_FREE_TEXTURE_UINT);
        GL_CHECK(glUniform1i(in_diffuse_location_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(in_normal_location_, geometry_buffer::NORMAL_METALNESS_LOCATION));
        GL_CHECK(glUniform1i(in_depth_stencil_location_, geometry_buffer::DEPTH_STENCIL_LOCATION));
        GL_CHECK(glUniform1i(in_shadow_map0_location_, geometry_buffer::SHADOW_MAP0_LOCATION));
        GL_CHECK(glUniform1i(in_shadow_map1_location_, geometry_buffer::SHADOW_MAP1_LOCATION));
        GL_CHECK(glUniform1i(in_shadow_map2_location_, geometry_buffer::SHADOW_MAP2_LOCATION));
    }

    void post_process_effect::apply(static_mesh_manager& static_mesh_mgr) const
    {
        STATIC_MESH_PTR(static_mesh_mgr, mesh_)->render_all();
    }
}
}

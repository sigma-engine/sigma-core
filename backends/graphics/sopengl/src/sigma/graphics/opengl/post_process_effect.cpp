#include <sigma/graphics/opengl/post_process_effect.hpp>

#include <sigma/graphics/opengl/geometry_buffer.hpp>
#include <sigma/graphics/opengl/static_mesh.hpp>
#include <sigma/graphics/opengl/util.hpp>

namespace sigma {
namespace opengl {
    post_process_effect::post_process_effect(const graphics::post_process_effect_data& data)
        : shader_technique<graphics::post_process_effect>(data)
    {
    }

    void post_process_effect::link()
    {
        shader_technique::link();
        GL_CHECK(in_diffuse_location_ = glGetUniformLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_INPUT_NAME));
        GL_CHECK(in_normal_location_ = glGetUniformLocation(object_, geometry_buffer::NORMAL_METALNESS_INPUT_NAME));
        GL_CHECK(in_depth_stencil_location_ = glGetUniformLocation(object_, geometry_buffer::DEPTH_STENCIL_INPUT_NAME));
    }

    void post_process_effect::bind() const
    {
        shader_technique::bind(geometry_buffer::NEXT_FREE_TEXTURE_UINT);
        GL_CHECK(glUniform1i(in_diffuse_location_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(in_normal_location_, geometry_buffer::NORMAL_METALNESS_LOCATION));
        GL_CHECK(glUniform1i(in_depth_stencil_location_, geometry_buffer::DEPTH_STENCIL_LOCATION));
    }

    void post_process_effect::apply() const
    {
        STATIC_MESH_CONST_PTR(mesh_)->render();
    }

    post_process_effect_manager::post_process_effect_manager(boost::filesystem::path cache_directory, opengl::shader_manager& shaders, opengl::texture_manager& textures, opengl::cubemap_manager& cubemaps, opengl::static_mesh_manager& meshes)
        : shader_technique_manager<post_process_effect, graphics::post_process_effect_manager>(cache_directory, shaders, textures, cubemaps)
        , meshes_(meshes)
    {
    }

    std::unique_ptr<graphics::post_process_effect> post_process_effect_manager::create(graphics::post_process_effect_data data)
    {
        auto effect = shader_technique_manager<post_process_effect, graphics::post_process_effect_manager>::create(std::move(data));
        effect->mesh().set_manager(&meshes_);
        return std::move(effect);
    }
}
}

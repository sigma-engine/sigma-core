#include <sigma/opengl/post_process_effect.hpp>

#include <sigma/opengl/geometry_buffer.hpp>
#include <sigma/opengl/static_mesh.hpp>
#include <sigma/opengl/util.hpp>

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

    void post_process_effect::bind()
    {
        shader_technique::bind(geometry_buffer::NEXT_FREE_TEXTURE_UINT);
        GL_CHECK(glUniform1i(in_diffuse_location_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION));
        GL_CHECK(glUniform1i(in_normal_location_, geometry_buffer::NORMAL_METALNESS_LOCATION));
        GL_CHECK(glUniform1i(in_depth_stencil_location_, geometry_buffer::DEPTH_STENCIL_LOCATION));
    }

    void post_process_effect::apply()
    {
        STATIC_MESH_PTR(mesh_)->render();
    }

    post_process_effect_manager::post_process_effect_manager(boost::filesystem::path cache_directory, opengl::texture_manager& textures, opengl::shader_manager& shaders, opengl::static_mesh_manager& meshes)
        : graphics::post_process_effect_manager(cache_directory)
        , textures_(textures)
        , shaders_(shaders)
        , meshes_(meshes)
    {
    }

    std::unique_ptr<graphics::post_process_effect> post_process_effect_manager::load(graphics::post_process_effect_data data, boost::archive::binary_iarchive& ia)
    {
        // TODO copy and pasted

        auto effect = std::make_unique<opengl::post_process_effect>(data);

        // TODO add other shader types here
        for (auto type : { graphics::shader_type::vertex, graphics::shader_type::fragment, graphics::shader_type::geometry }) {
            if (effect->has_shader(type))
                effect->shader(type).set_manager(&shaders_);
        }
        effect->link();
        for (unsigned int i = 0; i < effect->texture_count(); ++i)
            effect->texture(i).set_manager(&textures_);
        // TODO cubemap

        effect->mesh().set_manager(&meshes_);
        return std::move(effect);
    }
}
}

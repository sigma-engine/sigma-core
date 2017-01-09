#include <sigma/opengl/post_process_effect.hpp>

#include <sigma/opengl/geometry_buffer.hpp>
#include <sigma/opengl/static_mesh.hpp>
#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    post_process_effect::post_process_effect(resource::handle<graphics::static_mesh> mesh)
        : mesh_(mesh)
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
        auto effect = std::make_unique<opengl::post_process_effect>(meshes_.get(data.mesh));
        for (const auto& shdr : data.shaders)
            effect->attach(shaders_.get(shdr.second));
        effect->link();
        for (const auto& txt : data.textures)
            effect->set_texture(txt.first, textures_.get(txt.second));
        return std::move(effect);
    }
}
}

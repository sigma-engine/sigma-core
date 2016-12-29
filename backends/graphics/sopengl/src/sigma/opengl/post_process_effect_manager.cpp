#include <sigma/opengl/post_process_effect_manager.hpp>

#include <sigma/opengl/shader_manager.hpp>
#include <sigma/opengl/static_mesh_manager.hpp>
#include <sigma/opengl/texture_manager.hpp>

namespace sigma {
namespace opengl {
    /*std::shared_ptr<static_mesh> make_fullscreen_quad() 
	{
		// TODO use make shared
		return std::shared_ptr<static_mesh>(new static_mesh({ { { -1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f } },
		{ { -1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 1.0f } },
		{ { 1.0f, 1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 1.0f, 1.0f } },
		{ { 1.0f, -1.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 0.0f },{ 1.0f, 0.0f } } },
		{ { 0, 3, 2 },{ 2, 1, 0 } }, nullptr));
	}*/

    post_process_effect_manager::post_process_effect_manager(resource::resource_cache<graphics::post_process_effect>& cache, texture_manager& textures, shader_manager& shaders, static_mesh_manager& meshes)
        : resource::resource_manager<graphics::post_process_effect, opengl::post_process_effect>(cache)
        , textures_(textures)
        , shaders_(shaders)
        , meshes_(meshes)
    {
    }

    std::shared_ptr<opengl::post_process_effect> post_process_effect_manager::create(const graphics::post_process_effect& cpu_effect)
    {
        auto mat = std::make_shared<opengl::post_process_effect>(meshes_.get(cpu_effect.mesh()));
        for (const auto& shdr : cpu_effect.shaders)
            mat->attach(shaders_.get(shdr.second));
        mat->link();

        for (auto texture : cpu_effect.textures)
            mat->set_texture(texture.first, textures_.get(texture.second));

        return mat;
    }
}
}

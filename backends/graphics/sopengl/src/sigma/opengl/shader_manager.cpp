#include <sigma/opengl/shader_manager.hpp>

namespace sigma {
namespace opengl {
    shader_manager::shader_manager(resource::resource_cache<graphics::shader>& cache)
        : resource::resource_manager<graphics::shader, opengl::shader>(cache)
    {
    }

    std::shared_ptr<opengl::shader> shader_manager::create(const graphics::shader& cpu_shader)
    {
		shader_type type;
		switch (cpu_shader.type)
		{
		case graphics::shader_type::fragment: {
			type = shader_type::FRAGMENT_SHADER;
			break;
		}
		case graphics::shader_type::vertex: {
			type = shader_type::VERTEX_SHADER;
			break;
		}
		default: {
			// TODO support more than just vertex and fragment shaders
			assert("shader type is not supported");
			break;
		}
		}
        return std::make_shared<opengl::shader>(type,cpu_shader.source);
    }
}
}

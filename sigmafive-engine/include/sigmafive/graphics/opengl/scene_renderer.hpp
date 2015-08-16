#ifndef SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

#include <sigmafive/math/mat4.hpp>
#include <sigmafive/game/scene.hpp>
#include <sigmafive/graphics/opengl/shader.hpp>
#include <sigmafive/graphics/opengl/program.hpp>
#include <sigmafive/system/resource_manager.hpp>
#include <sigmafive/graphics/opengl/static_mesh.hpp>
#include <sigmafive/graphics/opengl/material.hpp>
#include <sigmafive/graphics/opengl/static_mesh_manager.hpp>

#include <memory>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class scene_renderer {
			public:
				scene_renderer(system::resource_manager &resource_manager);

				void render(float4x4 projection_matrix,float4x4 view_matrix,const game::scene &scene);
			private:
                opengl::material material_;

				opengl::shader vertex_shader;
				opengl::shader fragment_shader;

				system::resource_manager &resource_manager_;
				opengl::static_mesh_manager static_mesh_manager_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

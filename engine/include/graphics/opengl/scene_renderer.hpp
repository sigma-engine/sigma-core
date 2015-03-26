#ifndef SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

#include <math/mat4.hpp>
#include <game/scene.hpp>
#include <system/resource_manager.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class scene_renderer {
			public:
				scene_renderer(system::resource_manager &resource_ranager);

				void render(float4x4 view_matrix,const game::scene &scene);
			private:
				system::resource_manager &resource_manager_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

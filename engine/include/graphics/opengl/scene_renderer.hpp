#ifndef SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

#include <math/mat4.hpp>
#include <game/scene.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class scene_renderer {
			public:
				void render(float4x4 view_matrix,const game::scene &scene);
			private:
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

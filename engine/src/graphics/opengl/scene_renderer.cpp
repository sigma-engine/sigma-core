#include <graphics/opengl/scene_renderer.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			scene_renderer::scene_renderer(system::resource_manager &resource_ranager)
				: resource_manager_(resource_ranager) {
			}

			void scene_renderer::render(float4x4 view_matrix,const game::scene &scene) {
			}
		}
	}
}

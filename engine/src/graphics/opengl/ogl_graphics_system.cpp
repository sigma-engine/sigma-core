#include <graphics/opengl/ogl_graphics_system.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            ogl_graphics_system::ogl_graphics_system(sigmafive::context &ctx)
                : graphics_system(ctx) {
            }
		}
	}
}

SIGMAFIVE_IMPLEMENT_OBJECT(sigmafive::graphics::opengl::ogl_graphics_system)

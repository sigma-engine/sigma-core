#include <sigmafive/graphics/opengl/ogl_graphics_system.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            ogl_graphics_system::ogl_graphics_system(sigmafive::context &ctx)
                : graphics_system(ctx) {
            }
		}
	}
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::graphics::opengl::ogl_graphics_system)

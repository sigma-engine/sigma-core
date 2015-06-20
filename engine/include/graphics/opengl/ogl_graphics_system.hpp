#ifndef SIGMAFIVE_GRAPHICS_OPENGL_OGL_GRAPHICS_SYSTEM_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_OGL_GRAPHICS_SYSTEM_HPP

#include <graphics/graphics_system.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
            class ogl_graphics_system : public graphics_system {
                SIGMAFIVE_OBJECT()
            public:
                ogl_graphics_system(sigmafive::context &ctx);

                ogl_graphics_system(const ogl_graphics_system &) = default;
                ogl_graphics_system(ogl_graphics_system &&) = default;

                ogl_graphics_system &operator =(const ogl_graphics_system &) = default;
                ogl_graphics_system &operator =(ogl_graphics_system &&) = default;

                virtual ~ogl_graphics_system() = default;
            private:
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_OGL_GRAPHICS_SYSTEM_HPP

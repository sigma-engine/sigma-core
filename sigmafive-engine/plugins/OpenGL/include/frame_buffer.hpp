#ifndef SIGMAFIVE_GRAPHICS_OPENGL_FRAME_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_FRAME_BUFFER_HPP

#include <gl_core_4_0.hpp>

#include <cstdint>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class frame_buffer {
            public:
                frame_buffer();

                ~frame_buffer();
            private:
                GLuint gl_object_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_FRAME_BUFFER_HPP

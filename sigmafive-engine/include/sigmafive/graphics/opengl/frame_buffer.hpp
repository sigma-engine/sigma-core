#ifndef SIGMAFIVE_GRAPHICS_OPENGL_FRAME_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_FRAME_BUFFER_HPP

#include <cstdint>
#include <gl_core_4_0.hpp>

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

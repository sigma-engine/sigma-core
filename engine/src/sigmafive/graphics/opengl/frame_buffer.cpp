#include <graphics/opengl/frame_buffer.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            frame_buffer::frame_buffer() :
                gl_object_(0) {
                gl::GenFramebuffers(1,&gl_object_);//TODO GL_CHECK_ERROR;
            }

            frame_buffer::~frame_buffer() {
                gl::DeleteFramebuffers(1,&gl_object_);//TODO GL_CHECK_ERROR;
            }
		}
	}
}

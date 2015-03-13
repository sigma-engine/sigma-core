#include <graphics/opengl/vertex_buffer.hpp>
#include <gl_core_4_0.hpp>
#include <stdint-gcc.h>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			vertex_buffer_base::vertex_buffer_base(buffer_usage usage)
				: usage_(usage), gl_object_(0), size_(0), stride_(0) {
				gl::GenBuffers(1,&gl_object_);//TODO GL_CHECK_ERROR;
			}

			vertex_buffer_base::~vertex_buffer_base() {
				gl::DeleteBuffers(1,&gl_object_);//TODO GL_CHECK_ERROR; don't throw in the destructor
			}

			std::size_t vertex_buffer_base::size() const {
				return size_;
			}

			std::size_t vertex_buffer_base::stride() const {
				return stride_;
			}

			vertex_buffer_base::operator GLuint() const {
				return gl_object_;
			}

			void vertex_buffer_base::set_data(const void *data, std::size_t stride, std::size_t size) {
				//TODO preserve opengl buffer state??? or use direct state if is is available.
				gl::BindBuffer(gl::ARRAY_BUFFER,gl_object_); //TODO GL_CHECK_ERROR;
				if(stride * size != stride_ * size_) {
					gl::BufferData(gl::ARRAY_BUFFER, size * stride, data,static_cast<GLenum>(usage_));//TODO GL_CHECK_ERROR;
					size_ = size;
					stride_ = stride;
				}
				else {
					gl::BufferSubData(gl::ARRAY_BUFFER, 0, size * stride, data);//TODO GL_CHECK_ERROR;
				}
			}
		}
	}
}

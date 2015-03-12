#include <graphics/opengl/vertex_buffer.hpp>
#include <gl_core_4_0.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			namespace detail {
				static
				#if __cplusplus >= 201402L //TODO remove this when g++ has full support for c++14
				constexpr
				#endif
				GLenum convert_buffer_usage(buffer_usage usage) {
					switch (usage) {
						case stream_draw:
							return gl::STREAM_DRAW;
						case stream_read:
							return gl::STREAM_READ;
						case stream_copy:
							return gl::STREAM_COPY;

						case static_draw:
							return gl::STATIC_DRAW;
						case static_read:
							return gl::STATIC_READ;
						case static_copy:
							return gl::STATIC_COPY;

						case dynamic_draw:
							return gl::DYNAMIC_DRAW;
						case dynamic_read:
							return gl::DYNAMIC_READ;
						case dynamic_copy:
							return gl::DYNAMIC_COPY;
					}
				}
			}
			vertex_buffer::vertex_buffer(buffer_usage usage)
				: usage_(usage), gl_object_(0), size_(0), stride_(0) {
				gl::GenBuffers(1,&gl_object_);//TODO GL_CHECK_ERROR;
			}

			vertex_buffer::~vertex_buffer() {
				gl::DeleteBuffers(1,&gl_object_);//TODO GL_CHECK_ERROR; don't throw in the destructor
			}

			std::size_t vertex_buffer::size() const {
				return size_;
			}

			std::size_t vertex_buffer::stride() const {
				return stride_;
			}

			void vertex_buffer::set_data(const void *data, std::size_t stride, std::size_t size) {
				//TODO preserve opengl buffer state??? or use direct state if is is available.
				gl::BindBuffer(gl::ARRAY_BUFFER,gl_object_); //TODO GL_CHECK_ERROR;
				if(stride * size != stride_ * size_) {
					gl::BufferData(gl::ARRAY_BUFFER, size * stride, data, detail::convert_buffer_usage(usage_));//TODO GL_CHECK_ERROR;
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

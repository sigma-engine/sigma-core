#include <sigmafive/graphics/opengl/buffer.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            buffer::buffer(buffer_type type,buffer_usage usage)
                    : type_(type), usage_(usage), gl_object_(0), size_(0), stride_(0) {
                gl::GenBuffers(1,&gl_object_);//TODO GL_CHECK_ERROR;
            }

            buffer::~buffer() {
                gl::DeleteBuffers(1,&gl_object_);//TODO GL_CHECK_ERROR; don't throw in the destructor
            }

            std::size_t buffer::size() const {
                return size_;
            }

            std::size_t buffer::stride() const {
                return stride_;
            }

            buffer::operator GLuint() const {
                return gl_object_;
            }

            void buffer::set_data(const void *data, std::size_t stride, std::size_t size) {
                //TODO preserve opengl buffer state??? or use direct state if is is available.
                gl::BindBuffer(static_cast<GLenum>(type_),gl_object_); //TODO GL_CHECK_ERROR;
                if(stride * size != stride_ * size_) {
                    gl::BufferData(static_cast<GLenum>(type_), size * stride, data,static_cast<GLenum>(usage_));//TODO GL_CHECK_ERROR;
                    size_ = size;
                    stride_ = stride;
                }
                else {
                    gl::BufferSubData(static_cast<GLenum>(type_), 0, size * stride, data);//TODO GL_CHECK_ERROR;
                }
            }
        }
    }
}
#ifndef SIGMAFIVE_GRAPHICS_OPENGL_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_BUFFER_HPP

#include <gl_core_4_0.hpp>

#include <cstdint>
#include <cstdlib>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            enum class buffer_usage : GLenum {
                stream_draw = gl::STREAM_DRAW,
                stream_read = gl::STREAM_READ,
                stream_copy = gl::STREAM_COPY,
                static_draw = gl::STATIC_DRAW,
                static_read = gl::STATIC_READ,
                static_copy = gl::STATIC_COPY,
                dynamic_draw = gl::DYNAMIC_DRAW,
                dynamic_read = gl::DYNAMIC_READ,
                dynamic_copy = gl::DYNAMIC_COPY
            };

            enum class buffer_type : GLenum {
                array_buffer = gl::ARRAY_BUFFER,
                //TODO atomic_counter_buffer = gl::ATOMIC_COUNTER_BUFFER,
                        copy_read_buffer = gl::COPY_READ_BUFFER,
                copy_write_buffer = gl::COPY_WRITE_BUFFER,
                //TODO dispatch_indirect_buffer = gl::DISPATCH_INDIRECT_BUFFER,
                        draw_indirect_buffer = gl::DRAW_INDIRECT_BUFFER,
                element_array_buffer = gl::ELEMENT_ARRAY_BUFFER,
                pixel_pack_buffer = gl::PIXEL_PACK_BUFFER,
                pixel_unpack_buffer = gl::PIXEL_UNPACK_BUFFER,
                //TODO query_buffer = gl::QUERY_BUFFER,
                //TODO shader_storage_buffer = gl::SHADER_STORAGE_BUFFER,
                        texture_buffer = gl::TEXTURE_BUFFER,
                transform_feedback_buffer = gl::TRANSFORM_FEEDBACK_BUFFER,
                uniform_buffer = gl::UNIFORM_BUFFER
            };

            class buffer {
            public:
                buffer(buffer_type type, buffer_usage usage);

                virtual ~buffer();

                std::size_t size() const;

                std::size_t stride() const;

                operator GLuint() const;

            protected:
                void set_data(const void *data, std::size_t stride, std::size_t size);

            private:
                buffer_type type_;
                buffer_usage usage_;
                GLuint gl_object_;
                std::size_t size_;
                std::size_t stride_;
            };
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_BUFFER_HPP

#ifndef SIGMA_FIVE_OPENGL_BUFFER_HPP
#define SIGMA_FIVE_OPENGL_BUFFER_HPP

#include <sigma/opengl/config.hpp>

#include <cstdint>
#include <cstdlib>

#include <sigma/opengl/gl_core_4_0.h>

namespace sigma {
namespace opengl {
    enum class buffer_usage : GLenum {
        stream_draw = GL_STREAM_DRAW,
        stream_read = GL_STREAM_READ,
        stream_copy = GL_STREAM_COPY,
        static_draw = GL_STATIC_DRAW,
        static_read = GL_STATIC_READ,
        static_copy = GL_STATIC_COPY,
        dynamic_draw = GL_DYNAMIC_DRAW,
        dynamic_read = GL_DYNAMIC_READ,
        dynamic_copy = GL_DYNAMIC_COPY
    };

    enum class buffer_type : GLenum {
        array_buffer = GL_ARRAY_BUFFER,
        copy_read_buffer = GL_COPY_READ_BUFFER,
        copy_write_buffer = GL_COPY_WRITE_BUFFER,
        draw_indirect_buffer = GL_DRAW_INDIRECT_BUFFER,
        element_array_buffer = GL_ELEMENT_ARRAY_BUFFER,
        pixel_pack_buffer = GL_PIXEL_PACK_BUFFER,
        pixel_unpack_buffer = GL_PIXEL_UNPACK_BUFFER,
        texture_buffer = GL_TEXTURE_BUFFER,
        transform_feedback_buffer = GL_TRANSFORM_FEEDBACK_BUFFER,
        uniform_buffer = GL_UNIFORM_BUFFER
        // TODO atomic_counter_buffer = GL_ATOMIC_COUNTER_BUFFER,
        // TODO dispatch_indirect_buffer = GL_DISPATCH_INDIRECT_BUFFER,
        // TODO query_buffer = GL_QUERY_BUFFER,
        // TODO shader_storage_buffer = GL_SHADER_STORAGE_BUFFER,
    };

    class buffer {
    public:
        buffer(buffer_type type, buffer_usage usage);

        virtual ~buffer();

        std::size_t size() const;

        std::size_t stride() const;

        operator GLuint() const;

        GLuint gl_object_;

    protected:
        void set_data(const void* data, std::size_t stride, std::size_t size);

    private:
        buffer_type type_;
        buffer_usage usage_;
        std::size_t size_;
        std::size_t stride_;
    };
}
}

#endif // SIGMA_FIVE_OPENGL_BUFFER_HPP

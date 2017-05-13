#ifndef SIGMA_GRAPHICS_OPENGL_SHADOW_BUFFER_HPP
#define SIGMA_GRAPHICS_OPENGL_SHADOW_BUFFER_HPP

#include <sigma/graphics/opengl/frame_buffer.hpp>

#include <sigma/graphics/opengl/render_buffer.hpp>
#include <sigma/graphics/opengl/texture.hpp>

#include <glm/vec2.hpp>

namespace sigma {
namespace opengl {
    class shadow_buffer : public frame_buffer {
    public:
        shadow_buffer(glm::ivec2 size);

        void bind_for_shadow_write();

        void bind_for_shadow_read(texture_unit unit);

    private:
        texture shadow_map_;
        render_buffer depth_buffer_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_SHADOW_BUFFER_HPP

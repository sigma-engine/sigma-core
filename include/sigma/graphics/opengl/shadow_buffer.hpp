#ifndef SIGMA_GRAPHICS_OPENGL_SHADOW_BUFFER_HPP
#define SIGMA_GRAPHICS_OPENGL_SHADOW_BUFFER_HPP

#include <sigma/graphics/opengl/frame_buffer.hpp>

#include <sigma/config.hpp>
#include <sigma/graphics/opengl/render_buffer.hpp>
#include <sigma/graphics/opengl/texture.hpp>

#include <glm/vec2.hpp>

#include <vector>

namespace sigma {
namespace opengl {
    class shadow_buffer : public frame_buffer {
    public:
        shadow_buffer(glm::ivec2 resolution, std::size_t count);

        std::size_t count() const;

        void bind_for_shadow_write(int index);

        void bind_for_shadow_read(texture_unit unit);

    private:
        render_buffer depth_buffer_;
        std::vector<texture> shadow_maps_;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_SHADOW_BUFFER_HPP

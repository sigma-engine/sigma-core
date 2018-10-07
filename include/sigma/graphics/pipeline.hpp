#ifndef SIGMA_CORE_GRAPHICS_PIPELINE_HPP
#define SIGMA_CORE_GRAPHICS_PIPELINE_HPP

#include <sigma/frustum.hpp>
#include <sigma/graphics/texture.hpp>

#include <algorithm>
#include <functional>
#include <map>
#include <unordered_map>
#include <vector>

namespace sigma {
namespace graphics {
    struct depth_stencil_data {
        float depth;
        uint32_t stencil;
    };
    struct render_texture {
        glm::ivec2 size;
        texture_format format;
        glm::vec4 clear_color;
        depth_stencil_data depth_stencil;
        uint32_t id;
    };

    struct view_port {
        glm::ivec2 size;
        frustum view_frustum;
    };

    struct pass {
        view_port viewport;
        std::vector<uint32_t> in_textures;
        std::vector<uint32_t> out_textures;
        std::vector<bool> clear_textures;
        uint32_t id;
    };

    class pipeline {
    public:
        void add_render_texture(uint32_t id, render_texture t);

        const render_texture& get_render_texture(uint32_t id) const;

        void add_pass(uint32_t id, pass p);

        pass& get_pass(uint32_t id);

        std::map<uint32_t, pass>& passes();

    private:
        std::unordered_map<uint32_t, render_texture> render_textures_;
        std::map<uint32_t, pass> passes_;
    };
}
}

#endif // SIGMA_CORE_GRAPHICS_PIPELINE_HPP

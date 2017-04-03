#ifndef SIGMA_GRAPHICS_RENDERER_HPP
#define SIGMA_GRAPHICS_RENDERER_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/cubemap.hpp>
#include <sigma/graphics/frustum.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/texture.hpp>

namespace sigma {
namespace graphics {
    struct SIGMA_API view_port {
        glm::ivec2 size;
        frustum view_frustum;
    };

    class SIGMA_API renderer {
    public:
        renderer(glm::ivec2 size);

        virtual ~renderer();

        virtual texture_manager& textures() = 0;

        virtual cubemap_manager& cubemaps() = 0;

        virtual shader_manager& shaders() = 0;

        virtual material_manager& materials() = 0;

        virtual static_mesh_manager& static_meshes() = 0;

        virtual post_process_effect_manager& effects() = 0;

        virtual void resize(glm::uvec2 size) = 0;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
    };
}
}

#endif // SIGMA_GRAPHICS_RENDERER_HPP

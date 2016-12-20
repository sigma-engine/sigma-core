#ifndef sigma_GRAPHICS_OPENGL_CONTEXT_HPP
#define sigma_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/opengl/config.hpp>

#include <sigma/graphics/renderer.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/opengl/material.hpp>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/static_mesh.hpp>
#include <sigma/opengl/texture.hpp>

namespace sigma {
namespace opengl {
    class SIGMA_FIVE_OPENGL_API renderer : public graphics::renderer {
    public:
        static const resource::identifier PLANE_STATIC_MESH;
        static const resource::identifier FULLSCREEN_MATERIAL1;
        static const resource::identifier FULLSCREEN_MATERIAL2;

        renderer();

        virtual ~renderer();

        graphics::texture_cache& textures() override;

        graphics::shader_cache& shaders() override;

        graphics::material_cache& materials() override;

        static_mesh_cache& static_meshes() override;

        virtual void resize(glm::uvec2 size) override;

        virtual void render(const graphics::view_port& viewport) override;

    private:
        opengl::texture_cache textures_;
        opengl::shader_cache shaders_;
        opengl::material_cache materials_;
        opengl::static_mesh_cache static_meshes_;
    };
}
}

#endif // sigma_GRAPHICS_OPENGL_CONTEXT_HPP

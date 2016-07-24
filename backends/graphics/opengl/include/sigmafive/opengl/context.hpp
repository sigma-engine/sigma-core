#ifndef SIGMAFIVE_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigmafive/opengl/config.hpp>

#include <sigmafive/graphics/context.hpp>
#include <sigmafive/graphics/static_mesh_instance.hpp>
#include <sigmafive/opengl/material.hpp>
#include <sigmafive/opengl/shader.hpp>
#include <sigmafive/opengl/static_mesh.hpp>
#include <sigmafive/opengl/texture.hpp>

namespace sigmafive {
namespace opengl {
    class SIGMA_FIVE_OPENGL_API context : public graphics::context {
    public:
        static const resource::identifier PLANE_STATIC_MESH;
        static const resource::identifier FULLSCREEN_MATERIAL1;
        static const resource::identifier FULLSCREEN_MATERIAL2;

        context();

        virtual ~context();

        graphics::texture_cache& textures();

        graphics::shader_cache& shaders();

        graphics::material_cache& materials();

        static_mesh_cache& static_meshes();

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

#endif // SIGMAFIVE_GRAPHICS_OPENGL_CONTEXT_HPP

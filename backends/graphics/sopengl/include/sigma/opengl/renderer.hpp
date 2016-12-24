#ifndef sigma_GRAPHICS_OPENGL_CONTEXT_HPP
#define sigma_GRAPHICS_OPENGL_CONTEXT_HPP

#include <sigma/opengl/config.hpp>

#include <sigma/graphics/renderer.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/opengl/material.hpp>
#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/static_mesh.hpp>
#include <sigma/opengl/texture.hpp>

#include <sigma/graphics/material.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/texture.hpp>

#include <unordered_map>

namespace sigma {
namespace opengl {
    class renderer : public graphics::renderer {
    public:
        static constexpr const char* PROJECTION_MATRIX_NAME = "projectionMatrix";
        static constexpr const char* VIEW_MATRIX_NAME = "viewMatrix";
        static constexpr const char* MODEL_MATRIX_NAME = "modelMatrix";
        static constexpr const char* MODEL_VIEW_MATRIX_NAME = "modelViewMatrix";
        static constexpr const char* NORMAL_MATRIX_NAME = "normalMatrix";
        static constexpr const char* TIME_NAME = "time";

        static const resource::identifier PLANE_STATIC_MESH;
        static const resource::identifier FULLSCREEN_MATERIAL1;
        static const resource::identifier FULLSCREEN_MATERIAL2;

        renderer(context *ctx);

        virtual ~renderer();

        virtual void resize(glm::uvec2 size) override;

        opengl::texture get_texture(resource::identifier id);

        opengl::shader get_shader(resource::identifier id);

        opengl::material get_material(resource::identifier id);

        opengl::static_mesh get_static_mesh(resource::identifier id);

        virtual void render(const graphics::view_port& viewport) override;

    private:
        context *ctx_;
        graphics::texture_cache& texture_cache_;
		graphics::shader_cache& shader_cache_;
		graphics::material_cache& material_cache_;
		graphics::static_mesh_cache& static_mesh_cache_;

        std::unordered_map<resource::identifier,opengl::texture> textures_;
        std::unordered_map<resource::identifier,opengl::shader> shaders_;
        std::unordered_map<resource::identifier,opengl::material> materials_;
        std::unordered_map<resource::identifier,opengl::static_mesh> static_meshes_;
    };
}
}

#endif // sigma_GRAPHICS_OPENGL_CONTEXT_HPP

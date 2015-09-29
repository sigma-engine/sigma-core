#ifndef SIGMAFIVE_GRAPHICS_OPENGL_CONTEXT_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_CONTEXT_HPP

#if defined(_WIN32)
#define OPENGL_PLUGIN_API __declspec(dllexport)
#else
#define OPENGL_PLUGIN_API
#endif

#include <sigmafive/graphics/context.hpp>

#include <cppbr/opengl/shader.hpp>
#include <cppbr/opengl/program.hpp>

#include <g_buffer.hpp>
#include <material.hpp>
#include <static_mesh.hpp>
#include <static_mesh_manager.hpp>

#include <sigmafive/factory.hpp>

#include <queue>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            struct static_mesh_instance {
                float4x4 model_matrix;
                opengl::static_mesh *static_mesh;
            };
            class context : public graphics::context {
            SIGMAFIVE_CLASS()
            public:
                context(graphics::context_manager *manager);

                virtual void resize(uint2 size) override;

                virtual void add_static_mesh(float4x4 model_matrix,std::weak_ptr<graphics::static_mesh> static_mesh) override;

                virtual void render(float4x4 projection_matrix, float4x4 view_matrix) override;

                virtual void geometry_pass(float4x4 projection_matrix, float4x4 view_matrix);

                virtual void light_pass(float4x4 projection_matrix, float4x4 view_matrix);

                virtual void swap_buffers() override;

            private:
                uint2 size_;
                std::unique_ptr<g_buffer> g_buffer_;

                GLint drawFboId = 0;

                std::queue<static_mesh_instance> static_meshes_;

                opengl::material material_;
                cppbr::opengl::shader vertex_shader;
                cppbr::opengl::shader fragment_shader;

                cppbr::opengl::vertex_array plane_vertex_array;
                cppbr::opengl::index_buffer plane_index_buffer;
                cppbr::opengl::vertex_buffer<graphics::static_mesh::vertex> plane_vertex_buffer;

                cppbr::opengl::shader plane_vertex_shader;
                cppbr::opengl::shader plane_fragment_shader;
                cppbr::opengl::program plane_program;

                opengl::static_mesh_manager static_mesh_manager_;

            };

            using context_factory = sigmafive::simple_factory<sigmafive::graphics::context,context>;
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_CONTEXT_HPP

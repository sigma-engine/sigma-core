#include <context.hpp>

#include <sigmafive/engine.hpp>

#include <gl_core_4_0.hpp>

#include <memory>

#include <iostream>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            context::context()
                    : vertex_shader(shader_type::vertex), fragment_shader(shader_type::fragment),
                      plane_vertex_buffer(buffer_usage::static_draw),plane_index_buffer(buffer_usage::static_draw),
                      plane_vertex_shader(shader_type::vertex), plane_fragment_shader(shader_type::fragment) {

                plane_vertex_buffer.set_data({
                                                 {{-1.0f,-1.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,1.0f}},
                                                 {{-1.0f ,1.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f}},
                                                 {{1.0f ,1.0f ,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{1.0f,0.0f}},
                                                 {{1.0f,-1.0f ,0.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f},{1.0f,1.0f}}
                                             });
                plane_vertex_array.bind(plane_vertex_buffer, 0, &graphics::static_mesh::vertex::position);
                plane_vertex_array.bind(plane_vertex_buffer, 1, &graphics::static_mesh::vertex::normal);
                plane_vertex_array.bind(plane_vertex_buffer, 2, &graphics::static_mesh::vertex::tangent);
                plane_vertex_array.bind(plane_vertex_buffer, 3, &graphics::static_mesh::vertex::texcoord);

                plane_index_buffer.set_indicies({0,1,2,0,2,3});

                plane_vertex_shader.set_source(GLSL_440(
                                                   layout(location = 0) in vec3 vertex_position;
                                                   layout(location = 1) in vec3 vertex_normal;
                                                   layout(location = 2) in vec3 vertex_tangent;
                                                   layout(location = 3) in vec2 vertex_texcoord;

                                                   out vec3 position;
                                                   out vec3 normal;
                                                   out vec3 tangent;
                                                   out vec2 texcoord;

                                                   void main() {
                                                       position = vertex_position;
                                                       normal = vertex_normal;
                                                       tangent = vertex_tangent;
                                                       texcoord = vertex_texcoord;
                                                       gl_Position = vec4(vertex_position, 1);
                                                   }

                                                   ));
                plane_fragment_shader.set_source(GLSL_440(
                                                   uniform sampler2D in_color;
                                                   uniform vec2 screen_size;
                                                   in vec3 position;
                                                   in vec3 normal;
                                                   in vec3 tangent;
                                                   in vec2 texcoord;
                                                   layout (location = 0) out vec4 output_color;
                                                   vec2 screen_texcoord() {
                                                      return gl_FragCoord.xy / screen_size;
                                                   }
                                                   void main() {
                                                       output_color = texture(in_color,screen_texcoord());
                                                       output_color.a = 1.0;
                                                   }
                                                   ));
                plane_program.attach(plane_vertex_shader);
                plane_program.attach(plane_fragment_shader);
                auto msg = plane_program.link();
                if(msg != "")
                    throw std::runtime_error(msg);

                vertex_shader.set_source(GLSL_440(
                                         layout(location = 0) in vec3 vertex_position;
                                         layout(location = 1) in vec3 vertex_normal;
                                         layout(location = 2) in vec3 vertex_tangent;
                                         layout(location = 3) in vec2 vertex_texcoord;

                                         uniform mat4 projection_matrix;
                                         uniform mat4 view_matrix;
                                         uniform mat4 model_matrix;

                                         out vec3 position;
                                         out vec3 normal;
                                         out vec3 tangent;
                                         out vec2 texcoord;
                                         void main(void) {
                                             mat4 m = projection_matrix * view_matrix * model_matrix;
                                             gl_Position = m * vec4(vertex_position, 1);
                                             position = gl_Position.xyz;
                                             normal = (model_matrix * vec4(vertex_normal, 0)).xyz;
                                             tangent = (m * vec4(vertex_tangent, 0)).xyz;
                                             texcoord = vertex_texcoord;
                                         }));

                fragment_shader.set_source(GLSL_440(
                                               in vec3 position;
                                               in vec3 normal;
                                               in vec3 tangent;
                                               in vec2 texcoord;
                                               layout (location = 0) out vec4 g_position;
                                               layout (location = 1) out vec4 g_diffuse;
                                               layout (location = 2) out vec4 g_world_normal;
                                               layout (location = 3) out vec4 g_texcoord;
                                               void main() {
                                                   vec4 color = vec4(normalize(abs(normal)), 1);
                                                   g_position = vec4(position,1);
                                                   g_diffuse = color;
                                                   g_world_normal = vec4(normal,1);
                                                   g_texcoord = vec4(texcoord,0,1);
                                               }));

                material_.attach_shader(vertex_shader);
                material_.attach_shader(fragment_shader);
            }

            void context::make_current() {
            }

            void context::resize(uint2 size) {
                size_ = size;
                g_buffer_ = std::unique_ptr<g_buffer>{new g_buffer(size)};
            }

            void context::add_static_mesh(float4x4 model_matrix,std::weak_ptr<graphics::static_mesh> static_mesh) {
                auto gl_mesh = static_mesh_manager_.get(static_mesh.lock());
                static_meshes_.push({model_matrix, gl_mesh});
            }

            void context::render(float4x4 projection_matrix, float4x4 view_matrix) {
                gl::GetIntegerv(gl::DRAW_FRAMEBUFFER_BINDING, &drawFboId);

                gl::ClearColor(0, 0, 0, 1);

                geometry_pass(projection_matrix, view_matrix);
                light_pass(projection_matrix,view_matrix);
            }

            void context::geometry_pass(float4x4 projection_matrix, float4x4 view_matrix) {
                g_buffer_->frame_buffer_.bind(frame_buffer_target::draw);

                gl::Enable(gl::DEPTH_TEST);

                gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT);

                while (!static_meshes_.empty()) {
                    auto instance = static_meshes_.front();
                    auto static_mesh = instance.static_mesh;

                    material_.use();
                    material_.set_uniform("projection_matrix", projection_matrix);
                    material_.set_uniform("view_matrix", view_matrix);
                    material_.set_uniform("model_matrix", instance.model_matrix);

                    static_mesh->draw();

                    static_meshes_.pop();
                }
            }

            void context::light_pass(float4x4 projection_matrix, float4x4 view_matrix) {
                gl::BindFramebuffer(gl::DRAW_FRAMEBUFFER, drawFboId);

                gl::Disable(gl::DEPTH_TEST);
                gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT | gl::STENCIL_BUFFER_BIT);

                g_buffer_->frame_buffer_.bind(frame_buffer_target::read);

                g_buffer_->position_texture_.bind(0);
                g_buffer_->diffuse_texture_.bind(1);
                g_buffer_->normal_texture_.bind(2);
                g_buffer_->texcoord_texture_.bind(3);

                plane_program.use();

                plane_program.set_uniform("in_color",1);
                plane_program.set_uniform("screen_size",float2{size_.x,size_.y});
                plane_vertex_array.draw(primitive_type::triangles,plane_index_buffer,0,plane_index_buffer.size());
            }

            void context::swap_buffers() {

            }
        }
    }
}

//TODO this is a hack
extern "C" void OPENGL_PLUGIN_API register_plugin(sigmafive::engine *engine) {
    engine->graphics_context_manager()->register_context(sigmafive::graphics::opengl::context::CLASS_ID,
                                                        std::make_unique<sigmafive::graphics::opengl::context_factory>());
}


EXPORT_SIGMAFIVE_CLASS(sigmafive::graphics::opengl::context)

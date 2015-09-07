#include <sigmafive/graphics/opengl/context.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
            context::context(system::resource_manager &resource_manager)
                : vertex_shader(shader_type::vertex), fragment_shader(shader_type::fragment),
                  resource_manager_(resource_manager), static_mesh_manager_(resource_manager) {


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
                        mat4 m =   projection_matrix * view_matrix * model_matrix;
                        gl_Position = m * vec4(vertex_position,1);
                        position = gl_Position.xyz;
                        normal = (model_matrix * vec4(vertex_normal,0)).xyz;
                        tangent = (m * vec4(vertex_tangent,0)).xyz;
                        texcoord = vertex_texcoord;
                    }
                ));
                fragment_shader.set_source(GLSL_440(
                    in vec2 texcoord;
                    in vec3 normal;
                    void main() {
                        vec4 color = vec4(normalize(abs(normal)),1);
                        gl_FragColor = color;
                    }
                ));

                material_.attach_shader(vertex_shader);
                material_.attach_shader(fragment_shader);
			}

            void context::make_current() {

            }

            void context::render(float4x4 projection_matrix,float4x4 view_matrix,const game::scene &scene) {
				auto q = scene.static_meshes();

				while(!q.empty()) {
                    auto static_mesh = static_mesh_manager_.get(q.front().static_mesh->static_mesh);

                    float4x4 model_matrix = q.front().transform->matrix();

                    material_.use();
                    material_.set_uniform("projection_matrix",projection_matrix);
                    material_.set_uniform("view_matrix",view_matrix);
                    material_.set_uniform("model_matrix",model_matrix);

                    static_mesh->draw();

					q.pop();
				}
            }

            void context::swap_buffers() {

            }

            context_factory::context_factory(system::resource_manager &resource_manager)
                : resource_manager_(resource_manager) {
            }

            std::unique_ptr<graphics::context> context_factory::create() {
                return std::unique_ptr<context>(new context{resource_manager_});
            }
        }
	}
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::graphics::opengl::context)

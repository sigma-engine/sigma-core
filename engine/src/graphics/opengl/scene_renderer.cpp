#include <graphics/opengl/scene_renderer.hpp>


namespace sigmafive {
	namespace graphics {
		namespace opengl {
			scene_renderer::scene_renderer(system::resource_manager &resource_ranager)
				: vertex_shader(shader_type::vertex), fragment_shader(shader_type::fragment), resource_manager_(resource_ranager) {
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
							gl_Position = m * vec4(vertex_position,1);
							position = gl_Position.xyz;
							normal = (m * vec4(vertex_normal,0)).xyz;
							tangent = (m * vec4(vertex_tangent,0)).xyz;
							texcoord = vertex_texcoord;
						}
				));
				fragment_shader.set_source(std::string(GLSL_440(
						in vec2 texcoord;
						void main() {
							gl_FragColor = vec4(.5);
						}
				)));

				program.attach(vertex_shader);
				program.attach(fragment_shader);

				std::cout << program.link() << std::endl;
			}

			void scene_renderer::render(float4x4 projection_matrix,float4x4 view_matrix,const game::scene &scene) {
				auto q = scene.static_meshes();

				while(!q.empty()) {

					auto static_mesh_instance = q.front();
					q.pop();
					if(!meshes_[static_mesh_instance.static_mesh->static_mesh]) {
						auto mesh_resource = resource_manager_.get<sigmafive::graphics::static_mesh>(static_mesh_instance.static_mesh->static_mesh);
						meshes_[static_mesh_instance.static_mesh->static_mesh] = std::unique_ptr<opengl_static_mesh>(new opengl_static_mesh(mesh_resource.get()));
					}

					float4x4 model_matrix;
					program.use();
					program.set_uniform("projection_matrix",projection_matrix);
					program.set_uniform("view_matrix",view_matrix);
					program.set_uniform("model_matrix",model_matrix);
					meshes_[static_mesh_instance.static_mesh->static_mesh]->draw();
				}
			}
		}
	}
}

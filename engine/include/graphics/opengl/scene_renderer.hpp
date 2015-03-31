#ifndef SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

#include <math/mat4.hpp>
#include <game/scene.hpp>
#include <graphics/static_mesh.hpp>
#include <system/resource_manager.hpp>
#include <graphics/opengl/vertex_array.hpp>
#include <graphics/opengl/index_buffer.hpp>
#include <graphics/opengl/vertex_buffer.hpp>
#include <graphics/opengl/shader.hpp>
#include <graphics/opengl/program.hpp>

#include <memory>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class scene_renderer {
			public:
				scene_renderer(system::resource_manager &resource_ranager);

				void render(float4x4 projection_matrix,float4x4 view_matrix,const game::scene &scene);
			private:
				struct opengl_static_mesh {
					opengl::vertex_array vertex_array;
					opengl::index_buffer index_buffer;
					opengl::vertex_buffer<static_mesh::vertex> vertex_buffer;

					opengl_static_mesh(static_mesh *mesh_resource)
							: index_buffer(buffer_usage::static_draw),vertex_buffer(buffer_usage::static_draw) {
						vertex_buffer.set_data(mesh_resource->vertices());
						auto tris = mesh_resource->triangles();
						std::vector<unsigned int> indices(tris.size() * 3);
						unsigned int count = 0;
						for(auto tri:tris) {
							for(auto i:tri)
								indices[count++] = i;
						}
						index_buffer.set_indicies(indices);
						vertex_array.bind(vertex_buffer,0, &static_mesh::vertex::position);
						vertex_array.bind(vertex_buffer,1, &static_mesh::vertex::normal);
						vertex_array.bind(vertex_buffer,2, &static_mesh::vertex::tangent);
						vertex_array.bind(vertex_buffer,3, &static_mesh::vertex::texcoord);

					}

					void draw() {
						vertex_array.draw(sigmafive::graphics::opengl::primitive_type::triangles,index_buffer,0,index_buffer.size());
					}
				};
				opengl::shader vertex_shader;
				opengl::shader fragment_shader;
				opengl::program program;
				system::resource_manager &resource_manager_;
				std::unordered_map<boost::uuids::uuid,std::unique_ptr<opengl_static_mesh>,boost::hash<boost::uuids::uuid>> meshes_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

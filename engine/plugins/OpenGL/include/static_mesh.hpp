#ifndef SIGMAFIVE_GRAPHICS_OPENGL_STATIC_MESH_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_STATIC_MESH_HPP

#include <vertex_array.hpp>
#include <index_buffer.hpp>
#include <vertex_buffer.hpp>

#include <sigmafive/graphics/static_mesh.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            struct static_mesh {
                opengl::vertex_array vertex_array;
                opengl::index_buffer index_buffer;
                opengl::vertex_buffer<graphics::static_mesh::vertex> vertex_buffer;

                static_mesh(graphics::static_mesh *mesh_resource)
                        : index_buffer(buffer_usage::static_draw), vertex_buffer(buffer_usage::static_draw) {
                    vertex_buffer.set_data(mesh_resource->vertices());
                    auto tris = mesh_resource->triangles();
                    std::vector<unsigned int> indices(tris.size() * 3);
                    unsigned int count = 0;
                    for (auto tri:tris) {
                        for (auto i:tri)
                            indices[count++] = i;
                    }
                    index_buffer.set_indicies(indices);
                    vertex_array.bind(vertex_buffer, 0, &graphics::static_mesh::vertex::position);
                    vertex_array.bind(vertex_buffer, 1, &graphics::static_mesh::vertex::normal);
                    vertex_array.bind(vertex_buffer, 2, &graphics::static_mesh::vertex::tangent);
                    vertex_array.bind(vertex_buffer, 3, &graphics::static_mesh::vertex::texcoord);
                }

                void draw() {
                    vertex_array.draw(sigmafive::graphics::opengl::primitive_type::triangles, index_buffer, 0,
                                      index_buffer.size());
                }
            };
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_STATIC_MESH_HPP

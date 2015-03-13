#include <math/vec3.hpp>
#include <math/vec4.hpp>
#include <math/mat4.hpp>
#include <system/window.hpp>
#include <graphics/opengl/shader.hpp>
#include <graphics/opengl/program.hpp>
#include <graphics/opengl/buffer.hpp>
#include <graphics/opengl/vertex_buffer.hpp>
#include <graphics/opengl/vertex_array.hpp>

#include <iostream>

struct vertex {
    float3 position;
    float4 color;
};

int main(int argc, char const *argv[]) {
    sigmafive::system::context_attributes context_attributes;
    context_attributes.red = 8;
    context_attributes.green = 8;
    context_attributes.blue = 8;
    context_attributes.alpha = 8;
    context_attributes.depth = 24;
    context_attributes.stencil = 8;
    context_attributes.samples = 0;
    context_attributes.major = 4;
    context_attributes.minor = 4;
    context_attributes.double_buffer = true;
    context_attributes.core_profile = false;
    context_attributes.vsync = false;
    sigmafive::system::window window("Sigma Five", int2(800, 600), context_attributes);

    sigmafive::graphics::opengl::shader vertex_shader(sigmafive::graphics::opengl::shader_type::vertex);
    vertex_shader.set_source(GLSL_440(
            layout(location = 0) in vec3 vertex_position;
            layout(location = 1) in vec4 vertex_color;

            uniform mat4 projection_matrix;
            uniform mat4 view_matrix;
            uniform mat4 model_matrix;

            out vec4 vColor;
            void main(void) {
                vec4 a = vec4(vertex_position,1.0f);
                vColor = vertex_color;
                gl_Position = projection_matrix * view_matrix * model_matrix * a;
            }
    ));

    sigmafive::graphics::opengl::shader fragment_shader(sigmafive::graphics::opengl::shader_type::fragment);
    fragment_shader.set_source(std::string(GLSL_440(
            in vec4 vColor;

            void main() {
                gl_FragColor = vColor;
            }
    )));

    sigmafive::graphics::opengl::program program;

    program.attach(vertex_shader);
    program.attach(fragment_shader);

    std::cout << program.link() << std::endl;

    std::vector<vertex> vertices = {
            {{-1.0f,-1.0f,0.0f}, {1,0,0,1}},
            {{1.0f ,-1.0f,0.0f}, {0,1,0,1}},
            {{0.0f ,1.0f ,0.0f}, {0,0,1,1}}
    };

    sigmafive::graphics::opengl::vertex_buffer<vertex> vertex_buffer(sigmafive::graphics::opengl::buffer_usage::static_draw);
    vertex_buffer.set_data(vertices);


    sigmafive::graphics::opengl::vertex_array vertex_array;
    vertex_array.bind(vertex_buffer, 0, &vertex::position);
    vertex_array.bind(vertex_buffer, 1, &vertex::color);

    float4x4 projection_matrix;
    float4x4 view_matrix;
    float4x4 model_matrix;

    while(window.good()) {
        program.use();
        program.set_uniform("projection_matrix",projection_matrix);
        program.set_uniform("view_matrix",view_matrix);
        program.set_uniform("model_matrix",model_matrix);
        vertex_array.draw(sigmafive::graphics::opengl::primitive_type::triangle_strip,0,3);
    }

    return 0;
}

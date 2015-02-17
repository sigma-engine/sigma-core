#include <math/mat4.hpp>
#include <system/window.hpp>
#include <graphics/opengl/shader.hpp>
#include <graphics/opengl/program.hpp>

#include <iostream>

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

    float4x4 projection_matrix = float4x4::perspective(deg_to_rad(45.0f),800.0f/600.0f,0.01,1000.0f);
    float4x4 view_matrix;

    sigmafive::graphics::opengl::shader vertex_shader(sigmafive::graphics::opengl::shader::vertex_shader);
    vertex_shader.set_source(GLSL_440(
        in mat4 projection_matrix;
        in mat4 view_matrix;
        void main() {
        }
    ));


    sigmafive::graphics::opengl::shader fragment_shader(sigmafive::graphics::opengl::shader::fragment_shader);
    fragment_shader.set_source(std::string(GLSL_440(
        void main() {
        }
    )));

    sigmafive::graphics::opengl::program program;
    program.attach(vertex_shader);
    program.attach(fragment_shader);

    std::cout << program.link() << std::endl;

    while(window.good()) {
    }

    return 0;
}

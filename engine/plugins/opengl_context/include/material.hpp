#ifndef SIGMAFIVE_GRAPHICS_OPENGL_MATERIAL_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_MATERIAL_HPP

#include <cppbr/opengl/shader.hpp>
#include <cppbr/opengl/program.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            class material {
            public:
                void attach_shader(cppbr::opengl::shader &shader);

                void dettach_shader(cppbr::opengl::shader &shader);

                void use();

                template<typename T>
                void set_uniform(std::string name,T value) {
                    program_.set_uniform(name, value);
                }
            private:
                cppbr::opengl::program program_;
            };
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_MATERIAL_HPP

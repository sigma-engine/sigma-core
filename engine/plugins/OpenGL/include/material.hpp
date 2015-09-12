#ifndef SIGMAFIVE_GRAPHICS_OPENGL_MATERIAL_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_MATERIAL_HPP

#include <shader.hpp>
#include <program.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class material {
            public:
                void attach_shader(opengl::shader &shader);

                void dettach_shader(opengl::shader &shader);

                void use();

                template<typename T>
                void set_uniform(const std::string &name,const T &value) {
                    program_.set_uniform(name,value);
                }
            private:
                opengl::program program_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_MATERIAL_HPP

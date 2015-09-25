#include <material.hpp>

namespace sigmafive {
    namespace graphics {
        namespace opengl {

            void material::attach_shader(cppbr::opengl::shader &shader) {
                program_.attach(shader);
            }

            void material::dettach_shader(cppbr::opengl::shader &shader) {
                program_.detach(shader);
            }

            void material::use() {
                if (!program_.is_linked()) {
                    auto message = program_.link();
                    if (message.size() > 0) {
                        throw std::runtime_error(message);
                    }
                }

                program_.use();
            }
        }
    }
}

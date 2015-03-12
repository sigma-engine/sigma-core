#ifndef SIGMAFIVE_GRAPHICS_OPENGL_PROGRAM_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_PROGRAM_HPP

#include <graphics/opengl/shader.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <functional>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class program {
			public:
				program();

				~program();

				bool is_linked() const;

				void attach(shader &shader);

				void detach(shader &shader);

				std::string link();
			private:
				std::uint32_t gl_object_;
				bool linked_;
				std::vector<std::reference_wrapper<shader>> attached_shaders_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_PROGRAM_HPP

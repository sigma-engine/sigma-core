#ifndef SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_ARRAY_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_ARRAY_HPP

#include <cstdint>
#include <graphics/opengl/vertex_buffer.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class vertex_array {
			public:
				vertex_array();

				~vertex_array();

			private:
				std::uint32_t gl_object_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_ARRAY_HPP

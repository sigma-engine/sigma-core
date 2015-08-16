#ifndef SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_BUFFER_HPP

#include <sigmafive/graphics/opengl/buffer.hpp>

#include <vector>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class vertex_buffer_base : public buffer {
			public:
				vertex_buffer_base(buffer_usage usage) : buffer(buffer_type::array_buffer,usage) {}
			};

			template<typename T>
			class vertex_buffer : public vertex_buffer_base {
				static_assert(std::is_standard_layout<T>::value,"vertex type must be standard layout");
			public:
				vertex_buffer(buffer_usage usage) : vertex_buffer_base(usage) {}

				~vertex_buffer() {}

				void set_data(const std::vector<T> &data) {
					buffer::set_data(data.data(), sizeof(T), data.size());
				}
			private:
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_BUFFER_HPP
#ifndef SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_BUFFER_HPP

#include <type_traits>
#include <cstdint>
#include <vector>
#include <graphics/opengl/buffer.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class vertex_buffer {
			public:
				vertex_buffer(buffer_usage usage);

				~vertex_buffer();

				std::size_t size() const;

				std::size_t stride() const;

				template<typename T>
				typename std::enable_if<std::is_standard_layout<T>::value,void>::type
				set_data(const std::vector<T> &data) {
					set_data(static_cast<const void *>(data.data()), sizeof(T), data.size());
				}
			private:
				void set_data(const void *data,std::size_t stride,std::size_t size);

				buffer_usage usage_;
				std::uint32_t gl_object_;
				std::size_t size_;
				std::size_t stride_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_BUFFER_HPP

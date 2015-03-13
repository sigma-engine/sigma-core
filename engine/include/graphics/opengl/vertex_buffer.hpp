#ifndef SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_BUFFER_HPP

#include <type_traits>
#include <cstdint>
#include <vector>
#include <graphics/opengl/buffer.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {

			class vertex_buffer_base {
			public:
				vertex_buffer_base(buffer_usage usage);

				virtual ~vertex_buffer_base();

				std::size_t size() const;

				std::size_t stride() const;

			protected:
				void set_data(const void *data, std::size_t stride, std::size_t size);

			private:
				buffer_usage usage_;
				std::uint32_t gl_object_;
				std::size_t size_;
				std::size_t stride_;
			};

			template<typename T>
			class vertex_buffer : public vertex_buffer_base {
				static_assert(std::is_standard_layout<T>::value,"vertex type must be standard layout");
			public:
				vertex_buffer(buffer_usage usage) : vertex_buffer_base(usage) {}

				~vertex_buffer() {}

				void set_data(const std::vector<T> &data) {
					vertex_buffer_base::set_data(data.data(), sizeof(T), data.size());
				}
			private:
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_BUFFER_HPP

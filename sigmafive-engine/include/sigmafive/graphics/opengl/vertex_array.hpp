#ifndef SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_ARRAY_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_ARRAY_HPP

#include <cstdint>
#include <sigmafive/type_info.hpp>
#include <sigmafive/graphics/opengl/util.hpp>
#include <sigmafive/graphics/opengl/index_buffer.hpp>
#include <sigmafive/graphics/opengl/vertex_buffer.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			enum class primitive_type:GLenum {
				points = gl::POINTS,
				line_strip = gl::LINE_STRIP,
				line_loop = gl::LINE_LOOP,
				lines = gl::LINES,
				triangle_strip = gl::TRIANGLE_STRIP,
				triangle_fan = gl::TRIANGLE_FAN,
				triangles = gl::TRIANGLES
			};

			class vertex_array {
			public:
				vertex_array();

				~vertex_array();

				template<typename T>
				void bind(const vertex_buffer<T> &buffer, unsigned int index,bool normalize=false) {
					typedef component_info<T> info;
					bind(buffer, index, info::number_of_components,enum_type<typename info::component_type>::value,normalize,nullptr);
				}

				template<typename T,typename E>
				void bind(const vertex_buffer<T> &buffer, unsigned int index,E T::*offset_pointer,bool normalize=false) {
					typedef component_info<E> info;
					bind(buffer, index, info::number_of_components,enum_type<typename info::component_type>::value,normalize, (const void *)member_offset(offset_pointer));
				}

				void draw(primitive_type mode,std::size_t first,std::size_t count) const;

				void draw(primitive_type mode,const index_buffer &index_buffer,std::size_t first,std::size_t count) const;
			private:
				void bind(const vertex_buffer_base &buffer, unsigned int index, unsigned int number_of_components, data_types component_type, bool normalize, const void *offset);
				std::uint32_t gl_object_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_VERTEX_ARRAY_HPP

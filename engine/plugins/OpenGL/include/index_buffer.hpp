#ifndef SIGMAFIVE_GRAPHICS_OPENGL_INDEX_BUFFER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_INDEX_BUFFER_HPP

#include <buffer.hpp>

#include <vector>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			class index_buffer : public buffer {
			public:
				index_buffer(buffer_usage usage);

				virtual ~index_buffer();

				void set_indicies(const std::vector<unsigned int> &indices) {
					set_data(indices.data(),sizeof(unsigned int),indices.size());
				}
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_INDEX_BUFFER_HPP

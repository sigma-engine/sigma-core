#include <sigma/opengl/index_buffer.hpp>

namespace sigma {
namespace opengl {
    index_buffer::index_buffer(buffer_usage usage)
        : buffer(buffer_type::element_array_buffer, usage)
    {
    }

    index_buffer::~index_buffer()
    {
    }
}
}

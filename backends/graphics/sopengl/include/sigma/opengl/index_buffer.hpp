#ifndef SIGMA_FIVE_OPENGL_INDEX_BUFFER_HPP
#define SIGMA_FIVE_OPENGL_INDEX_BUFFER_HPP

#include <sigma/opengl/config.hpp>

#include <array>
#include <vector>

#include <sigma/opengl/buffer.hpp>

namespace sigma {
namespace opengl {
    class index_buffer : public buffer {
    public:
        index_buffer(buffer_usage usage);

        virtual ~index_buffer();

        void set_triangles(const std::vector<std::array<unsigned int, 3> >& triangles)
        {
            set_data(triangles.data(), sizeof(unsigned int), 3 * triangles.size());
        }

        void set_indicies(const std::vector<unsigned int>& indices)
        {
            set_data(indices.data(), sizeof(unsigned int), indices.size());
        }
    };
}
}

#endif // SIGMA_FIVE_OPENGL_INDEX_BUFFER_HPP

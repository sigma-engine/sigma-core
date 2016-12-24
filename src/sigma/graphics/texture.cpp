#include <sigma/graphics/texture.hpp>

#include <fstream>
#include <iostream>
#include <string>
#include <algorithm>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>

namespace sigma {
namespace graphics {

    unsigned int texture::width() const { return width_; }

    unsigned int texture::height() const { return height_; }

    const std::vector<unsigned char>& texture::data() const { return data_; }

    void texture::set_data(unsigned int width, unsigned int height,
        const std::vector<unsigned char>& data)
    {
        this->width_ = width;
        this->height_ = height;
        this->data_ = std::move(data);
    }
}
}

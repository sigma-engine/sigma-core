#include <sigma/graphics/texture.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>

namespace sigma {
namespace graphics {

    unsigned int texture::width() const { return width_; }

    unsigned int texture::height() const { return height_; }

    glm::ivec2 texture::size() const
    {
        return glm::ivec2{ width_, height_ };
    }

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

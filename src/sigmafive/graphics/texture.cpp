#include <sigmafive/graphics/texture.hpp>

namespace sigmafive {
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

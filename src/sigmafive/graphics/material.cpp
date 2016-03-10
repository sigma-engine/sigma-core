#include <sigmafive/graphics/material.hpp>

namespace sigmafive {
namespace graphics {
    const std::vector<resource::identifier>& material::get_shaders() const
    {
        return shaders_;
    }

    void material::add_shader(resource::identifier shader)
    {
        shaders_.push_back(shader);
    }

    void material::remove_shader(resource::identifier shader)
    {
        auto end = std::remove(shaders_.begin(), shaders_.end(), shader);
        shaders_.erase(end, shaders_.end());
    }
}
}

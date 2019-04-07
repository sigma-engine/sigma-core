#include <sigma/graphics/renderer.hpp>

#include <filesystem>

#include <fstream>

namespace sigma {
namespace graphics {
    renderer::renderer(glm::ivec2 size, std::shared_ptr<context> ctx)
        : context_(ctx),
        settings_(ctx->settings<renderer::settings>())
    {
    }

    renderer::~renderer() {}
}
}

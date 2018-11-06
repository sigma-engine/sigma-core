#include <sigma/graphics/renderer.hpp>

namespace sigma {
namespace graphics {
    renderer::renderer(glm::ivec2 size, std::shared_ptr<context> ctx)
        : context_(ctx)
    {
    }
}
}

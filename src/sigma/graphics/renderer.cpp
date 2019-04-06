#include <sigma/graphics/renderer.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <filesystem>

#include <fstream>

namespace sigma {
namespace graphics {
    renderer::renderer(glm::ivec2 size, context_view_type ctx)
        : context_(ctx)
        , settings_(ctx.get_settings<settings>())
    {
    }

    renderer::~renderer() {}
}
}

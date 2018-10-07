#include <sigma/graphics/pipeline.hpp>

#include <cassert>

namespace sigma {
namespace graphics {

    void pipeline::add_render_texture(uint32_t id, render_texture tex)
    {
        assert(render_textures_.count(id) == 0);
        tex.id = id;
        render_textures_[id] = tex;
    }

    const render_texture& pipeline::get_render_texture(uint32_t id) const
    {
        return render_textures_.at(id);
    }

    void pipeline::add_pass(uint32_t id, pass p)
    {
        assert(passes_.count(id) == 0);
        p.id = id;
        passes_[id] = p;
    }

    pass& pipeline::get_pass(uint32_t id)
    {
        return passes_.at(id);
    }

    std::map<uint32_t, pass>& pipeline::passes()
    {
        return passes_;
    }
}
}

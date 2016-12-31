#ifndef SIGMA_CONTEXT_HPP
#define SIGMA_CONTEXT_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/texture.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <boost/filesystem/path.hpp>

#include <memory>
#include <unordered_map>

namespace sigma {
class game;
struct game_class;
namespace graphics {
    class renderer;
    struct renderer_class;
}
class SIGMA_API context {
public:
    context();

    virtual ~context() = default;

    graphics::texture_cache& textures();

    graphics::shader_cache& shaders();

    graphics::post_process_effect_cache& effects();

    graphics::material_cache& materials();

    graphics::static_mesh_cache& static_meshes();

    bool load_plugin(boost::filesystem::path path);

    std::shared_ptr<graphics::renderer> create_renderer(std::string renderer_class, glm::ivec2 size);

    std::shared_ptr<game> create_game(std::string game_class);

protected:
    graphics::texture_cache textures_;
    graphics::shader_cache shaders_;
    graphics::material_cache materials_;
    graphics::static_mesh_cache static_meshes_;
    graphics::post_process_effect_cache effects_;

    std::unordered_map<std::string, boost::shared_ptr<game_class> > game_classes;
    std::unordered_map<std::string, boost::shared_ptr<graphics::renderer_class> > renderer_classes;
};
}

#endif // SIGMA_CONTEXT_HPP

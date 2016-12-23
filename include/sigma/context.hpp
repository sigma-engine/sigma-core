#ifndef SIGMA_CONTEXT_HPP
#define SIGMA_CONTEXT_HPP

#include <sigma/config.hpp>

#include <boost/filesystem/path.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <memory>
#include <unordered_map>

namespace sigma {
class game;
struct game_class;
namespace graphics {
    class renderer;
    struct renderer_class;
    class texture_cache;
    class shader_cache;
    class material_cache;
    class static_mesh_cache;
}
class SIGMA_API context {
public:
    context();

    virtual ~context();

    graphics::texture_cache& textures();

    graphics::shader_cache& shaders();

    graphics::material_cache& materials();

    graphics::static_mesh_cache& static_meshes();

    bool load_plugin(boost::filesystem::path path);

    void set_renderer_class(std::string renderer_class);

    void set_game_class(std::string game_class);

    void update(std::chrono::duration<float> dt);

    virtual void render(glm::ivec2 size);

protected:
    // TODO custom pointer that works better with dynamic libs
    std::shared_ptr<game> current_game;
    std::shared_ptr<graphics::renderer> current_renderer;

private:
    float z = 0;
    std::unordered_map<std::string, boost::shared_ptr<game_class> > game_classes;
    std::unordered_map<std::string, boost::shared_ptr<graphics::renderer_class> > renderer_classes;
};
}

#endif // SIGMA_CONTEXT_HPP

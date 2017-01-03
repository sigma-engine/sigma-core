#include <sigma/context.hpp>

#include <sigma/game.hpp>
#include <sigma/graphics/renderer.hpp>

#include <boost/dll/import.hpp>

namespace sigma {
context::context()
//    : textures_(boost::filesystem::current_path() / ".." / "data")
//    , shaders_(boost::filesystem::current_path() / ".." / "data")
//    , materials_(boost::filesystem::current_path() / ".." / "data", textures_, shaders_)
//    , static_meshes_(boost::filesystem::current_path() / ".." / "data", materials_)
//    , effects_(boost::filesystem::current_path() / ".." / "data", textures_, shaders_, static_meshes_)
{
}

bool context::load_plugin(boost::filesystem::path path)
{
    uint32_t total = 0;
    uint32_t i = 0;
    while (true) {
        try {
            auto klass = boost::dll::import<graphics::renderer_class>(path, "renderer_class_" + std::to_string(i), boost::dll::load_mode::append_decorations);
            renderer_classes[klass->name] = klass;
            i++;
            total++;
        } catch (boost::system::system_error e) {
            break;
        }
    }
    i = 0;
    while (true) {
        try {
            auto klass = boost::dll::import<game_class>(path, "game_class_" + std::to_string(i), boost::dll::load_mode::append_decorations);
            game_classes[klass->name] = klass;
            i++;
            total++;
        } catch (boost::system::system_error e) {
            break;
        }
    }
    return total != 0;
}

std::shared_ptr<graphics::renderer> context::create_renderer(std::string renderer_class, glm::ivec2 size)
{
    return renderer_classes[renderer_class]->create(size);
}

std::shared_ptr<game> context::create_game(std::string game_class, std::shared_ptr<graphics::renderer> renderer)
{
    return game_classes[game_class]->create(renderer.get());
}
}

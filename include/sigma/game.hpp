#ifndef SIGMA_GAME_HPP
#define SIGMA_GAME_HPP

#include <sigma/entity_manager.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/transform.hpp>

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#include <chrono>
#include <memory>

#define SIGMA_EXPORT_GAME_CLASS_I(r, data, i, elem)                                                                          \
    extern "C" BOOST_SYMBOL_EXPORT sigma::game_class game_class_##i;                                                         \
    sigma::game_class game_class_##i = {                                                                                     \
        BOOST_PP_STRINGIZE(elem),                                                                                            \
        [](sigma::graphics::renderer* renderer) -> std::shared_ptr<sigma::game> { return std::make_shared<elem>(renderer); } \
    };

#define SIGMA_EXPORT_GAME_CLASSES(...) BOOST_PP_SEQ_FOR_EACH_I(SIGMA_EXPORT_GAME_CLASS_I, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

namespace sigma {
class context;
class SIGMA_API game {
public:
    sigma::entity_manager entities;

    transform_manager transforms;

    graphics::directional_light_manager directional_lights;

    graphics::point_light_manager point_lights;

    graphics::spot_light_manager spot_lights;

    graphics::static_mesh_instance_manager static_mesh_instances;

    game(graphics::renderer* renderer);

    virtual ~game() = default;

    void load(boost::filesystem::path file_path);

    virtual void update(std::chrono::duration<float> dt) = 0;

protected:
    graphics::renderer* renderer_;
};

struct game_class {
    const char* name;
    std::shared_ptr<sigma::game> (*create)(sigma::graphics::renderer* renderer);
};
}

#endif // SIGMA_GAME_HPP

#ifndef SIGMA_GAME_HPP
#define SIGMA_GAME_HPP

#include <sigma/entity_manager.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/reflect/reflect.hpp>
#include <sigma/transform.hpp>

//#include <boost/dll/alias.hpp>

#include <chrono>
#include <memory>

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#define SIGMA_EXPORT_GAME_CLASS_I(r, data, i, elem)                                                     \
    extern "C" BOOST_SYMBOL_EXPORT sigma::game_class game_class_##i;                                    \
    sigma::game_class game_class_##i = {                                                                \
        BOOST_PP_STRINGIZE(elem),                                                                       \
        [](sigma::context* ctx) -> std::shared_ptr<sigma::game> { return std::make_shared<elem>(ctx); } \
    };

#define SIGMA_EXPORT_GAME_CLASSES(...) BOOST_PP_SEQ_FOR_EACH_I(SIGMA_EXPORT_GAME_CLASS_I, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

namespace sigma {
class context;
class SIGMA_API game {
public:
    RPROPERTY()
    graphics::texture_cache& textures;

    RPROPERTY()
    graphics::shader_cache& shaders;

    RPROPERTY()
    graphics::material_cache& materials;

    RPROPERTY()
    graphics::static_mesh_cache& static_meshes;

    RPROPERTY()
    sigma::entity_manager entities;

    RPROPERTY()
    transform_manager transforms;

    RPROPERTY()
    graphics::directional_light_manager directional_lights;

    RPROPERTY()
    graphics::point_light_manager point_lights;

    RPROPERTY()
    graphics::spot_light_manager spot_lights;

    RPROPERTY()
    graphics::static_mesh_instance_manager static_mesh_instances;

    game(context* ctx);

    virtual ~game() = default;

    virtual void update(std::chrono::duration<float> dt) = 0;

protected:
    context* ctx;
};

struct game_class {
    const char* name;
    std::shared_ptr<sigma::game> (*create)(sigma::context* ctx);
};
}

#endif // SIGMA_GAME_HPP

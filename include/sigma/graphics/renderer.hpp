#ifndef SIGMA_GRAPHICS_RENDERER_HPP
#define SIGMA_GRAPHICS_RENDERER_HPP

#include <sigma/config.hpp>
#include <sigma/context.hpp>
#include <sigma/entity_manager.hpp>
#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/material.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/graphics/scene.hpp>
#include <sigma/graphics/shader.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/transform.hpp>
#include <sigma/graphics/cubemap.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#define SIGMA_EXPORT_RENDERER_CLASS_I(r, data, i, elem)                                                      \
    extern "C" BOOST_SYMBOL_EXPORT sigma::graphics::renderer_class renderer_class_##i;                       \
    sigma::graphics::renderer_class renderer_class_##i = {                                                   \
        BOOST_PP_STRINGIZE(elem),                                                                            \
        [](glm::ivec2 s) -> std::shared_ptr<sigma::graphics::renderer> { return std::make_shared<elem>(s); } \
    };

#define SIGMA_EXPORT_RENDERER_CLASSES(...) BOOST_PP_SEQ_FOR_EACH_I(SIGMA_EXPORT_RENDERER_CLASS_I, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

namespace sigma {
namespace graphics {
    struct SIGMA_API view_port {
        entity_manager& entities;
        transform_manager& transforms;
        static_mesh_instance_manager& static_mesh_instances;
        point_light_manager& point_lights;
        directional_light_manager& directional_lights;
        spot_light_manager& spot_lights;
        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;
        float z_near;
        float z_far;
        glm::vec2 size;
    };

    class SIGMA_API renderer {
    public:
        renderer(glm::ivec2 size);

        virtual ~renderer();

        virtual texture_manager& textures() = 0;

        virtual cubemap_manager& cubemaps() = 0;

        virtual shader_manager& shaders() = 0;

        virtual material_manager& materials() = 0;

        virtual static_mesh_manager& static_meshes() = 0;

        virtual post_process_effect_manager& effects() = 0;

        virtual std::shared_ptr<scene> create_scene() = 0;

        virtual void resize(glm::uvec2 size) = 0;

        virtual void render(const view_port& viewport) = 0;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
    };

    struct renderer_class {
        const char* name;
        std::shared_ptr<renderer> (*create)(glm::ivec2);
    };
}
}

#endif // SIGMA_GRAPHICS_RENDERER_HPP

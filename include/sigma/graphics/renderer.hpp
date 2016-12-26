#ifndef SIGMA_GRAPHICS_RENDERER_HPP
#define SIGMA_GRAPHICS_RENDERER_HPP

#include <sigma/config.hpp>
#include <sigma/context.hpp>
#include <sigma/entity_manager.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/graphics/post_process_effect.hpp>
#include <sigma/transform.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <boost/preprocessor/seq/for_each_i.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>

#define SIGMA_EXPORT_RENDERER_CLASS_I(r, data, i, elem)                                                                                \
    extern "C" BOOST_SYMBOL_EXPORT sigma::graphics::renderer_class renderer_class_##i;                                                 \
    sigma::graphics::renderer_class renderer_class_##i = {                                                                             \
        BOOST_PP_STRINGIZE(elem),                                                                                                      \
        [](sigma::context* ctx, glm::ivec2 s) -> std::shared_ptr<sigma::graphics::renderer> { return std::make_shared<elem>(ctx, s); } \
    };

#define SIGMA_EXPORT_RENDERER_CLASSES(...) BOOST_PP_SEQ_FOR_EACH_I(SIGMA_EXPORT_RENDERER_CLASS_I, _, BOOST_PP_VARIADIC_TO_SEQ(__VA_ARGS__))

namespace sigma {
namespace graphics {
    //    // PSEUDO code for occlusion query culling
    //    auto
    //    software_occlusion_cull(depthbuff,occluders_begin,occluders_end,instances_begin,instances_end)
    //    {
    //        std::for_each(occluders_begin,occluders_end,cpu_render(depthbuff,_1));
    //        return
    //        std::stable_partition(instances_begin,instances_end,cpu_render(depthbuff,_1));
    //    }

    struct SIGMA_API view_port {
        entity_manager& entities;
        transform_manager& transforms;
        static_mesh_instance_manager& static_mesh_instances;
        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;
    };

    class SIGMA_API renderer {
    public:
        renderer(context* ctx, glm::ivec2 size);

        virtual ~renderer();

        virtual void resize(glm::uvec2 size) = 0;

        //        // PSEUDO code for occlusion culling
        //        virtual void render(const view_port &viewport) {
        //            if(viewport.dirty)
        //                octree.update(viewport,viewport.static_mesh_instances.begin(),viewport.static_mesh_instances.end());
        //            else
        //                octree.update(viewport,viewport.static_mesh_instances.dirty_begin(),viewport.static_mesh_instances.dirty_end());

        //            sorted.resize(octree.visable_size());
        //            std::copy(octree.visable_begin(),octree.visable_end());
        //            std::stable_sort(sorted.begin(),sorted.end(),...);
        //            auto end =
        //            graphics::software_occlusion_cull(occuders.begin(),occuders.end(),sorted.begin(),sorted.end());
        //            render(viewport,sorted.begin(),sorted.end());
        //        }

        virtual void render(const view_port& viewport) = 0;

    private:
        renderer(const renderer&) = delete;
        renderer& operator=(const renderer&) = delete;
    };

    struct renderer_class {
        const char* name;
        std::shared_ptr<renderer> (*create)(sigma::context*, glm::ivec2);
    };
}
}

#endif // SIGMA_GRAPHICS_RENDERER_HPP

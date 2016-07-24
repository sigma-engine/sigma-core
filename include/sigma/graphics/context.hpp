#ifndef SIGMA_ENGINE_GRAPHICS_CONTEXT_HPP
#define SIGMA_ENGINE_GRAPHICS_CONTEXT_HPP

#include <sigma/config.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

#include <sigma/entity_manager.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/transform.hpp>

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

    struct sigma_API view_port {
        entity_manager& entities;
        transform_manager& transforms;
        static_mesh_instance_manager& static_mesh_instances;
        glm::mat4 projection_matrix;
        glm::mat4 view_matrix;
    };

    class sigma_API context {
    public:
        context();

        virtual ~context();

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
    };
}
}

#endif // SIGMA_ENGINE_GRAPHICS_CONTEXT_HPP

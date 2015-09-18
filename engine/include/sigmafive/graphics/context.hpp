#ifndef SIGMAFIVE_GRAPHICS_CONTEXT_HPP
#define SIGMAFIVE_GRAPHICS_CONTEXT_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/object.hpp>

#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/mat4.hpp>

#include <sigmafive/graphics/static_mesh.hpp>

#include <boost/uuid/uuid.hpp>

namespace sigmafive {
    namespace graphics {
        class context_manager;

        class SIGMAFIVE_API context : public object {
        SIGMAFIVE_CLASS()
        public:
            context(graphics::context_manager *manager);

            virtual ~context() = default;

            graphics::context_manager *context_manager();

            virtual void make_current();

            virtual void resize(uint2 size) = 0;

            virtual void add_static_mesh(float4x4 model_matrix,std::weak_ptr<graphics::static_mesh> static_mesh) = 0;

            virtual void render(float4x4 projection_matrix, float4x4 view_matrix) = 0;

            virtual void swap_buffers() = 0;
        private:
            graphics::context_manager *context_manager_;
        };
    }
}

#endif //SIGMAFIVE_GRAPHICS_CONTEXT_HPP

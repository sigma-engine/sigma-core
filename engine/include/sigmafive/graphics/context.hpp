#ifndef SIGMAFIVE_GRAPHICS_CONTEXT_HPP
#define SIGMAFIVE_GRAPHICS_CONTEXT_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/object.hpp>

#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/mat4.hpp>

#include <boost/uuid/uuid.hpp>
#include "static_mesh.hpp"

namespace sigmafive {
    namespace graphics {
        class SIGMAFIVE_API context : public object {
        SIGMAFIVE_CLASS()
        public:
            virtual ~context() = default;

            virtual void resize(uint2 size) = 0;

            //TODO calling make current on context should make it current in the context manager
            virtual void make_current() = 0;

            virtual void add_static_mesh(float4x4 model_matrix,std::weak_ptr<graphics::static_mesh> static_mesh) = 0;

            virtual void render(float4x4 projection_matrix, float4x4 view_matrix) = 0;

            virtual void swap_buffers() = 0;
        private:
        };
    }
}

#endif //SIGMAFIVE_GRAPHICS_CONTEXT_HPP

#ifndef SIGMAFIVE_GRAPHICS_CONTEXT_HPP
#define SIGMAFIVE_GRAPHICS_CONTEXT_HPP

#include <sigmafive/object.hpp>

#include <sigmafive/math/mat4.hpp>
#include <sigmafive/game/scene.hpp>

namespace sigmafive {
	namespace graphics {
		class context : public object {
            SIGMAFIVE_CLASS()
		public:
			virtual ~context() = default;

            virtual void make_current() = 0;

            virtual void render(float4x4 projection_matrix,float4x4 view_matrix,const game::scene &scene) = 0;

            virtual void swap_buffers() = 0;
		private:
		};
	}
}

#endif //SIGMAFIVE_GRAPHICS_CONTEXT_HPP

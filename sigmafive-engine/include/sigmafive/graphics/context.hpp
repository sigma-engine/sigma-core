#ifndef SIGMAFIVE_GRAPHICS_CONTEXT_HPP
#define SIGMAFIVE_GRAPHICS_CONTEXT_HPP

#include <sigmafive/object.hpp>
#include <sigmafive/math/mat4.hpp>

#include <queue>
#include <boost/uuid/uuid.hpp>

namespace sigmafive {
	namespace graphics {
		class context : public object {
            SIGMAFIVE_CLASS()
		public:
			virtual ~context() = default;

            virtual void make_current() = 0;

            virtual void add_static_mesh(float4x4 model_matrix,boost::uuids::uuid static_mesh) = 0;

            //TODO remove this???
            virtual void render(float4x4 projection_matrix,float4x4 view_matrix) = 0;

            virtual void swap_buffers() = 0;
		private:
		};
	}
}

#endif //SIGMAFIVE_GRAPHICS_CONTEXT_HPP

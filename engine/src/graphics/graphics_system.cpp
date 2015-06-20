#include <graphics/graphics_system.hpp>

namespace sigmafive {
    namespace graphics {
        graphics_system::graphics_system(sigmafive::context &ctx) :
            context_(ctx) {
            context_.set_system<graphics_system>(*this);
        }

        graphics_system::~graphics_system() {
            context_.remove_system(*this);
        }

        context &graphics_system::context() {
            return context_;
        }
	}
}

SIGMAFIVE_IMPLEMENT_OBJECT_NOT_SERIALIZABLE(sigmafive::graphics::graphics_system)

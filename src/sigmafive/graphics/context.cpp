#include <sigmafive/graphics/context.hpp>
#include <sigmafive/graphics/context_manager.hpp>

namespace sigmafive {
    namespace graphics {
        context::context(graphics::context_manager *manager)
            : context_manager_(manager) {
        }

        void context::make_current() {
            context_manager_->make_current(this);
        }

        graphics::context_manager *context::context_manager() {
            return context_manager_;
        }
    }
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::graphics::context)
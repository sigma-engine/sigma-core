#include <sigmafive/graphics/context.hpp>
#include <sigmafive/graphics/context_manager.hpp>

namespace sigmafive {
    namespace graphics {
        context::context(context_manager *manager)
            : context_manager_(manager) {
        }

        void context::make_current() {
            context_manager_->make_current(this);
        }

        graphics::context_manager *context::get_context_manager() {
            return context_manager_;
        }
    }
}
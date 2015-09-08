#include <sigmafive/graphics/context_manager.hpp>

namespace sigmafive {
    namespace graphics {
        context_manager::context_manager()
            : current_context_(nullptr) {
        }

        void context_manager::register_context(class_uid uid, std::unique_ptr<context_factory> factory) {
                context_factories_[uid] = std::move(factory);
            //TODO maybe an error when trying to register a factory more than once
        }

        std::unique_ptr<context> context_manager::create_context(class_uid uid) {
            //TODO error when if the context type does not exits
            return std::move(context_factories_.at(uid)->create());
        }

        void context_manager::make_current(context *ctx) {
            current_context_ = ctx;
        }

        context *context_manager::current_context() {
            return current_context_;
        }

        void context_manager::unregister_context(class_uid uid) {
            context_factories_.erase(uid);
            //TODO maybe an error when the class is not registered
        }
	}
}

EXPORT_SIGMAFIVE_CLASS(sigmafive::graphics::context_manager)

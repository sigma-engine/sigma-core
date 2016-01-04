#include <sigmafive/graphics/context_manager.hpp>
#include <sigmafive/graphics/context.hpp>

#include <stdexcept>

namespace sigmafive {
    namespace graphics {
        context_manager::context_manager()
                : current_context_(nullptr) {
        }

        void context_manager::register_context(cppbr::meta::class_uid uid, std::unique_ptr<context_factory> factory) {
            if (context_factories_.count(uid) != 0)
                throw std::runtime_error(
                        "graphics context with uid = " + std::to_string(uid) + " was already registered!");
            context_factories_[uid] = std::move(factory);
        }

        std::unique_ptr<context> context_manager::create_context(cppbr::meta::class_uid uid) {
            if (context_factories_.count(uid) == 0)
                throw std::runtime_error("graphics context with uid = " + std::to_string(uid) + " was not registered!");
            return std::move(context_factories_.at(uid)->create(this));
        }

        void context_manager::make_current(context *ctx) {
            current_context_ = ctx;
        }

        context *context_manager::current_context() {
            return current_context_;
        }

        void context_manager::unregister_context(cppbr::meta::class_uid uid) {
            if (context_factories_.count(uid) == 0)
                throw std::runtime_error("graphics context with uid = " + std::to_string(uid) + " was not registered!");
            context_factories_.erase(uid);
        }
    }
}

EXPORT_CPPBR_META_CLASS(sigmafive::graphics::context_manager)

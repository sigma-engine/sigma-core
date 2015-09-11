#ifndef SIGMAFIVE_GRAPHICS_CONTEXT_MANGER_HPP
#define SIGMAFIVE_GRAPHICS_CONTEXT_MANGER_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/object.hpp>
#include <sigmafive/factory.hpp>

#include <sigmafive/graphics/context.hpp>

#include <memory>
#include <unordered_map>

namespace sigmafive {
	namespace graphics {
        using context_factory = factory<context>;
        class SIGMAFIVE_API context_manager : public object {
            SIGMAFIVE_CLASS()
        public:
            context_manager();

            void register_context(class_uid uid,std::unique_ptr<context_factory> factory);

            //TODO share_ptr
            std::unique_ptr<context> create_context(class_uid uid);

            //TODO weak_ptr
            void make_current(context *ctx);

            //TODO weak_ptr
            context *current_context();

            void unregister_context(class_uid uid);
        private:
            //TODO weak ptr
            context *current_context_;
            std::unordered_map<class_uid,std::unique_ptr<context_factory>> context_factories_;
		};
	}
}

#endif //SIGMAFIVE_GRAPHICS_CONTEXT_MANGER_HPP

#ifndef SIGMAFIVE_GRAPHICS_CONTEXT_MANGER_HPP
#define SIGMAFIVE_GRAPHICS_CONTEXT_MANGER_HPP

#include <sigmafive/object.hpp>
#include <sigmafive/factory.hpp>
#include <sigmafive/graphics/context.hpp>

#include <memory>
#include <unordered_map>

namespace sigmafive {
	namespace graphics {
        using context_factory = factory<context>;
        class context_manager : public object {
            SIGMAFIVE_CLASS()
        public:
            void register_context(class_uid uid,std::unique_ptr<context_factory> factory);

            std::unique_ptr<context> create_context(class_uid uid);

            void unregister_context(class_uid uid);
        private:            
            std::unordered_map<class_uid,std::unique_ptr<context_factory>> context_factories_;
		};
	}
}

#endif //SIGMAFIVE_GRAPHICS_CONTEXT_MANGER_HPP

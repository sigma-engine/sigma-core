#ifndef SIGMAFIVE_GRAPHICS_CONTEXT_MANGER_HPP
#define SIGMAFIVE_GRAPHICS_CONTEXT_MANGER_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>
#include <cppbr/meta/factory.hpp>

#include <sigmafive/graphics/context.hpp>

#include <memory>
#include <unordered_map>

namespace sigmafive {
    class engine;
    namespace graphics {
        class context_manager;
        class context;

        class SIGMAFIVE_API context_factory {
        public:
            virtual ~context_factory() = default;

            virtual std::unique_ptr<context> create(context_manager *manager) = 0;
        };

        class SIGMAFIVE_API context_manager {
        public:
            context_manager();

            context_manager(const context_manager &) = delete;

            context_manager &operator=(const context_manager &) = delete;

            void register_context(cppbr::meta::class_uid uid, std::unique_ptr<context_factory> factory);

            //TODO share_ptr
            std::unique_ptr<context> create_context(cppbr::meta::class_uid uid);

            //TODO weak_ptr
            void make_current(context *ctx);

            //TODO weak_ptr
            context *current_context();

            void unregister_context(cppbr::meta::class_uid uid);

        private:
            //TODO weak ptr
            context *current_context_;
            std::unordered_map<cppbr::meta::class_uid, std::unique_ptr<context_factory>> context_factories_;
        };

        template<class T>
        class simple_context_factory : public context_factory {
        public:
            virtual ~simple_context_factory() = default;

            virtual std::unique_ptr<context> create(context_manager *manager) override  {
                return std::move(std::make_unique<T>(manager));
            }
        private:
        };
    }
}

#endif //SIGMAFIVE_GRAPHICS_CONTEXT_MANGER_HPP

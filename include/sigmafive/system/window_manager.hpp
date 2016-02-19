#ifndef SIGMAFIVE_SYSTEM_WINDOW_MANAGER_HPP
#define SIGMAFIVE_SYSTEM_WINDOW_MANAGER_HPP

#include <cppbr/meta/object.hpp>
#include <cppbr/meta/factory.hpp>

#include <sigmafive/system/window.hpp>

namespace sigmafive {
    namespace system {
        using window_factory = cppbr::meta::factory<window>;

        class SIGMAFIVE_API window_manager {
        public:
            window_manager() = default;

            window_manager(const window_manager &) = delete;

            window_manager &operator=(const window_manager &) = delete;

            void register_window(cppbr::meta::class_uid uid, std::unique_ptr<window_factory> factory);

            //TODO share_ptr
            std::unique_ptr<window> create_window(cppbr::meta::class_uid uid);

            void unregister_context(cppbr::meta::class_uid uid);

        private:
            std::unordered_map<cppbr::meta::class_uid, std::unique_ptr<window_factory>> window_factories_;
        };
    }
}

#endif //SIGMAFIVE_SYSTEM_WINDOW_MANAGER_HPP

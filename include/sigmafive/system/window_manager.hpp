#ifndef SIGMAFIVE_SYSTEM_WINDOW_MANAGER_HPP
#define SIGMAFIVE_SYSTEM_WINDOW_MANAGER_HPP

#include <cppbr/meta/object.hpp>
#include <sigmafive/factory.hpp>

#include <sigmafive/system/window.hpp>

namespace sigmafive {
    namespace system {
        using window_factory = factory<window>;

        class SIGMAFIVE_API window_manager : public cppbr::meta::object {
        CPPBR_META_CLASS()
        public:
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

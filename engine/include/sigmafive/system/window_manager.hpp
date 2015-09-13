#ifndef SIGMAFIVE_SYSTEM_WINDOW_MANAGER_HPP
#define SIGMAFIVE_SYSTEM_WINDOW_MANAGER_HPP

#include <sigmafive/object.hpp>
#include <sigmafive/factory.hpp>

#include <sigmafive/system/window.hpp>

namespace sigmafive {
    namespace system {
        using window_factory = factory<window>;

        class SIGMAFIVE_API window_manager : public object {
            SIGMAFIVE_CLASS()
        public:
            void register_window(class_uid uid,std::unique_ptr<window_factory> factory);

            //TODO share_ptr
            std::unique_ptr<window> create_window(class_uid uid);

            void unregister_context(class_uid uid);
        private:
            std::unordered_map<class_uid,std::unique_ptr<window_factory>> window_factories_;
        };
    }
}

#endif //SIGMAFIVE_SYSTEM_WINDOW_MANAGER_HPP

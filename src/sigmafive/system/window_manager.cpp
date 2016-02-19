#include <sigmafive/system/window_manager.hpp>

#include <stdexcept>

namespace sigmafive {
    namespace system {
        void window_manager::register_window(cppbr::meta::class_uid uid, std::unique_ptr<window_factory> factory) {
            if (window_factories_.count(uid) != 0)
                throw std::runtime_error("window with uid = " + std::to_string(uid) + " was already registered!");
            window_factories_[uid] = std::move(factory);
        }

        std::unique_ptr<window> window_manager::create_window(cppbr::meta::class_uid uid) {
            if (window_factories_.count(uid) == 0)
                throw std::runtime_error("window with uid = " + std::to_string(uid) + " was not registered!");
            return std::move(window_factories_.at(uid)->create());
        }

        void window_manager::unregister_context(cppbr::meta::class_uid uid) {
            if (window_factories_.count(uid) == 0)
                throw std::runtime_error("window with uid = " + std::to_string(uid) + " was not registered!");
            window_factories_.erase(uid);
        }
    }
}
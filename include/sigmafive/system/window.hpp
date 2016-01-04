#ifndef SIGMAFIVE_SYSTEM_WINDOW_HPP
#define SIGMAFIVE_SYSTEM_WINDOW_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>

namespace sigmafive {
    namespace system {
        class SIGMAFIVE_API window : public cppbr::meta::object {
        CPPBR_META_CLASS()
        public:
            virtual ~window() = default;

            virtual bool good() = 0;

            virtual void close() = 0;

        private:
        };
    }
}

#endif //SIGMAFIVE_SYSTEM_WINDOW_HPP

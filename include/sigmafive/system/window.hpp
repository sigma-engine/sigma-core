#ifndef SIGMAFIVE_SYSTEM_WINDOW_HPP
#define SIGMAFIVE_SYSTEM_WINDOW_HPP

#include <sigmafive/config.hpp>

namespace sigmafive {
    namespace system {
        class SIGMAFIVE_API window {
        public:
            virtual ~window() = default;

            virtual bool good() = 0;

            virtual void close() = 0;

        private:
        };
    }
}

#endif //SIGMAFIVE_SYSTEM_WINDOW_HPP

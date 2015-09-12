#ifndef SIGMAONE_SYSTEM_WINDOW_HPP
#define SIGMAONE_SYSTEM_WINDOW_HPP

#include <sigmafive/config.hpp>

#include <sigmafive/math/vec2.hpp>

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

#endif //SIGMAONE_SYSTEM_WINDOW_HPP

#ifndef SIGMAFIVE_GRAPHICS_GRAPHICS_SYSTEM_HPP
#define SIGMAFIVE_GRAPHICS_GRAPHICS_SYSTEM_HPP

#include <sigmafive/object.hpp>

namespace sigmafive {
	namespace graphics {
        class graphics_system : public object {
            SIGMAFIVE_CLASS()
        public:
            graphics_system();

            graphics_system(const graphics_system &) = default;
            graphics_system(graphics_system &&) = default;

            graphics_system &operator =(const graphics_system &) = default;
            graphics_system &operator =(graphics_system &&) = default;

            virtual ~graphics_system();
        private:            
		};
	}
}

#endif //SIGMAFIVE_GRAPHICS_GRAPHICS_SYSTEM_HPP

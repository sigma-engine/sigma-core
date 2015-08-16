#ifndef SIGMAFIVE_GRAPHICS_GRAPHICS_SYSTEM_HPP
#define SIGMAFIVE_GRAPHICS_GRAPHICS_SYSTEM_HPP

#include <object.hpp>
#include <context.hpp>

namespace sigmafive {
	namespace graphics {
        class graphics_system : public object {
            SIGMAFIVE_CLASS()
        public:
            graphics_system(sigmafive::context &ctx);

            graphics_system(const graphics_system &) = default;
            graphics_system(graphics_system &&) = default;

            graphics_system &operator =(const graphics_system &) = default;
            graphics_system &operator =(graphics_system &&) = default;

            virtual ~graphics_system();

            sigmafive::context &context();
        private:
            sigmafive::context &context_;
		};
	}
}

#endif //SIGMAFIVE_GRAPHICS_GRAPHICS_SYSTEM_HPP

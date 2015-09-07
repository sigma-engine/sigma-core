#ifndef SIGMAONE_SYSTEM_WINDOW_HPP
#define SIGMAONE_SYSTEM_WINDOW_HPP

#include <sigmafive/math/vec2.hpp>
#include <sigmafive/graphics/opengl/context.hpp>

#include <string>

struct SDL_Window;

namespace sigmafive {
	namespace system {
        struct context_attributes {
            int red;
            int green;
            int blue;
            int alpha;
            int depth;
            int stencil;
            int samples;
            int major;
            int minor;
            bool double_buffer;
            bool core_profile;
            bool vsync;
        };

		class window {
        public:
            window(std::string title, int2 size, context_attributes context_attributes);

            ~window();

            bool good();

            void close();

        private:
            std::string title_;
            int2 size_;
            context_attributes context_attributes_;
            SDL_Window *window_;
            void *gl_context_;
            bool good_;
        };
	}
}

#endif //SIGMAONE_SYSTEM_WINDOW_HPP

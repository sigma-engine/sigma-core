#ifndef SIGMAONE_SYSTEM_WINDOW_HPP
#define SIGMAONE_SYSTEM_WINDOW_HPP

#include <context.hpp>
#include <math/vec2.hpp>
#include <graphics/opengl/ogl_graphics_system.hpp>

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

            sigmafive::context &context();

            bool good();

            void close();

        private:
            sigmafive::context context_;
            sigmafive::graphics::opengl::ogl_graphics_system graphics_system_;

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

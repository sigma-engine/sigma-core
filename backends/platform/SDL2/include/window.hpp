#ifndef SIGMA_SDL2_WINDOW_HPP
#define SIGMA_SDL2_WINDOW_HPP

#include <string>

#include <glm/vec2.hpp>

struct SDL_Window;

namespace sigma {
namespace system {
    namespace SDL2 {
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
            window();

            ~window();

            bool good();

            void close();

        private:
            std::string title_;
            glm::ivec2 size_;
            SDL_Window* window_;
            void* gl_context_;
            bool good_;
        };
    }
}
}

#endif //SIGMA_SDL2_WINDOW_HPP

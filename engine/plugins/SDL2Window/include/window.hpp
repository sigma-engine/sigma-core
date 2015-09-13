#ifndef SDL2_WINDOW_HPP
#define SDL2_WINDOW_HPP


#include <sigmafive/system/window.hpp>

#include <sigmafive/math/vec2.hpp>

#include <string>

struct SDL_Window;

namespace sigmafive {
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

            class window : public sigmafive::system::window {
            SIGMAFIVE_CLASS()
            public:
                window();

                ~window();

                bool good() override;

                void close() override;
            private:
                std::string title_;
                int2 size_;
                SDL_Window *window_;
                void *gl_context_;
                bool good_;
            };
        }
    }
}

#endif //SDL2_WINDOW_HPP

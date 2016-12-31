#ifndef SDL2_WINDOW_HPP
#define SDL2_WINDOW_HPP

#include <glm/vec2.hpp>

#include <string>

struct SDL_Window;

namespace sigma {
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
    window(glm::ivec2 size);

    ~window();

    glm::ivec2 size();

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

#endif //SDL2_WINDOW_HPP

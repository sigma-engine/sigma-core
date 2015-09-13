#include <window.hpp>

#include <sigmafive/engine.hpp>

#include <SDL.h>

#include <unordered_map>

namespace sigmafive {
    namespace system {
        namespace SDL2 {
            namespace detail {
                void init_sdl2_if_not_already() {
                    if (SDL_WasInit(SDL_INIT_EVERYTHING) == 0) {
                        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
                            throw std::runtime_error(SDL_GetError());
                        }
                    }
                }

                static std::unordered_map<Uint32, window *> created_windows;
            }

            window::window()
                    : title_(""), size_(int2(800, 600)), good_(false) {
                detail::init_sdl2_if_not_already();

                //SDL_GL_SetAttribute(SDL_GL_RED_SIZE, context_attributes_.red);
                //SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, context_attributes_.green);
                //SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, context_attributes_.blue);
                //SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, context_attributes_.alpha);
                //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, context_attributes_.depth);
                //SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, context_attributes_.stencil);

                //if(context_attributes_.samples > 0) {
                //    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
                //    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, context_attributes_.samples);
                //}

                //SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, context_attributes_.double_buffer);

                //SDL_GL_SetSwapInterval(context_attributes_.vsync);

                //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, context_attributes_.major);
                //SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, context_attributes_.minor);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

                //if(context_attributes_.core_profile)
                //    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);

                SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

                window_ = SDL_CreateWindow(title_.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size_.x,
                                           size_.y, SDL_WINDOW_OPENGL);
                if (!window_)
                    throw std::runtime_error(SDL_GetError());

                gl_context_ = SDL_GL_CreateContext(window_);
                if (!gl_context_)
                    throw std::runtime_error(SDL_GetError());

                SDL_GL_MakeCurrent(window_, gl_context_);

                good_ = true;
                detail::created_windows[SDL_GetWindowID(window_)] = this;
            }

            window::~window() {
                close();
            }

            bool window::good() {
                SDL_Event event;
                while (SDL_PollEvent(&event) != 0) {
                    switch (event.type) {
                        case SDL_WINDOWEVENT: {
                            auto event_window = detail::created_windows[event.window.windowID];
                            switch (event.window.event) {
                                case SDL_WINDOWEVENT_CLOSE:
                                    event_window->good_ = false;
                                    event_window->close();
                                    break;
                            }
                            break;
                        }
                    }
                }
                SDL_GL_MakeCurrent(window_, gl_context_);
                //if(context_attributes_.double_buffer)
                SDL_GL_SwapWindow(window_);
                return good_;
            }

            void window::close() {
                if (window_) {
                    if (gl_context_) {
                        SDL_GL_DeleteContext(gl_context_);
                        gl_context_ = nullptr;
                    }
                    SDL_DestroyWindow(window_);
                    window_ = nullptr;
                }
            }
        }
    }
}
EXPORT_SIGMAFIVE_CLASS(sigmafive::system::SDL2::window)

//TODO this is a hack
extern "C" {
    void __declspec(dllexport) register_plugin(sigmafive::engine *engine) {
        engine->window_manager().register_window(sigmafive::system::SDL2::window::CLASS_ID,
                                                 std::unique_ptr<sigmafive::simple_factory<sigmafive::system::window,sigmafive::system::SDL2::window>>{
                                                         new sigmafive::simple_factory<sigmafive::system::window,sigmafive::system::SDL2::window>{}});
    }
}
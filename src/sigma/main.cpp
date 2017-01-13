#include <simple_game.hpp>

#include <sigma/opengl/renderer.hpp>
#include <sigma/trackball_controller.hpp>
#include <sigma/window.hpp>

#include <SDL.h>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

int main(int argc, char* argv[])
{
    sigma::window window(glm::ivec2{ 1920, 1080 });

    auto renderer = std::make_unique<sigma::opengl::renderer>(window.size());
    auto game = std::make_unique<simple_game>(renderer.get());

    sigma::graphics::view_port viewport{
        game->entities,
        game->transforms,
        game->static_mesh_instances,
        game->point_lights,
        game->directional_lights,
        game->spot_lights,
        glm::perspective(0.785398f, (float)window.size().x / (float)window.size().y, 0.01f, 10000.0f),
        glm::mat4(1),
        0.01f,
        10000.0f,
        window.size()
    };
    sigma::trackball_controller controller;
    auto start = std::chrono::high_resolution_clock::now();
    std::size_t count = 0;
    while (window.good()) {
        viewport.view_matrix = controller.matrix();
        if (renderer) {
            SDL_GL_MakeCurrent(window.window_, window.gl_context_);
            renderer->render(viewport);
        }

        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_MOUSEWHEEL: {
                controller.zoom(event.wheel.y);
                break;
            }
            case SDL_MOUSEMOTION: {
                controller.update(float(event.motion.x) / window.size().x, float(event.motion.y) / window.size().y);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                if (!controller.is_rotating())
                    controller.beginRotate(float(event.motion.x) / window.size().x, float(event.motion.y) / window.size().y);
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                if (controller.is_rotating())
                    controller.endRotate(float(event.motion.x) / window.size().x, float(event.motion.y) / window.size().y);
                if (controller.isPanning())
                    controller.endPan();
                break;
            }
            case SDL_KEYDOWN: {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    game = nullptr;
                    renderer = nullptr;
                    window.close();
                }
                if (!controller.isPanning())
                    controller.beginPan();
                break;
            }
            case SDL_KEYUP: {
                if (controller.isPanning())
                    controller.endPan();
                break;
            }
            case SDL_WINDOWEVENT: {
                switch (event.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    game = nullptr;
                    renderer = nullptr;
                    window.close();
                    break;
                }
            case SDL_WINDOWEVENT_RESIZED: {
                break;
            }
            }
            }
        }

        SDL_GL_SwapWindow(window.window_);
        count++;
        auto end = std::chrono::high_resolution_clock::now();
        auto dt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (dt > 1000ms) {
            auto fps = count / std::chrono::duration_cast<std::chrono::duration<float>>(dt).count();
            std::cout << "Frame time: " << (dt.count() / count) << " ms" << std::endl;
            std::cout << "FPS: " << fps << std::endl;
            start = end;
            count = 0;
        }
    }
    return 0;
}

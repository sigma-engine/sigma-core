#include <sigma/context.hpp>
#include <sigma/game.hpp>
#include <sigma/graphics/renderer.hpp>
#include <sigma/trackball_controller.hpp>
#include <sigma/window.hpp>

#include <SDL.h>

#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

int main(int argc, char const* argv[])
{
    sigma::context ctx;
    sigma::window window(glm::ivec2{1920,1080});

    ctx.load_plugin("sopengl");
    auto renderer = ctx.create_renderer("sigma::opengl::renderer", window.size());

    ctx.load_plugin("simple-game");
    auto game = ctx.create_game("simple_game");

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
    while (window.good()) {
        viewport.view_matrix = controller.matrix();
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            switch (event.type) {
            case SDL_MOUSEWHEEL: {
                controller.zoom(event.wheel.y);
                break;
            }
            case SDL_MOUSEMOTION: {
                controller.update(float(event.motion.x)/window.size().x,float(event.motion.y)/window.size().y);
                break;
            }
            case SDL_MOUSEBUTTONDOWN: {
                if(!controller.is_rotating())
                    controller.beginRotate(float(event.motion.x)/window.size().x,float(event.motion.y)/window.size().y);
                break;
            }
            case SDL_MOUSEBUTTONUP: {
                if(controller.is_rotating())
                    controller.endRotate(float(event.motion.x)/window.size().x,float(event.motion.y)/window.size().y);
                if(controller.isPanning())
                    controller.endPan();
                break;
            }
            case SDL_KEYDOWN: {
                if(!controller.isPanning())
                    controller.beginPan();
                break;
            }
            case SDL_KEYUP: {
                if(controller.isPanning())
                    controller.endPan();
                break;
            }
            case SDL_WINDOWEVENT: {
                switch (event.window.event) {
                case SDL_WINDOWEVENT_CLOSE:
                    window.close();
                    break;
                }
                case SDL_WINDOWEVENT_RESIZED: {
                    break;
                }
                }
            }
        }

        renderer->render(viewport);
    }
    return 0;
}

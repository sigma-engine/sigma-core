#pragma once

#include <sigmafive/config.hpp>
#include <sigmafive/engine.hpp>

#include <memory>

#define REGISTER_GAME(game_class,API) \
extern "C" API void register_plugin(sigmafive::engine *engine) { \
    engine->set_game(std::make_shared<game_class>(engine));\
}\
extern "C" API void unregister_plugin(sigmafive::engine *engine) { \
    if(dynamic_cast<game_class*>(engine->game().get()))\
        engine->set_game(nullptr);\
}

namespace sigmafive {
    class engine;

    namespace graphics { class contex; }

    namespace game {
        class SIGMAFIVE_API game {
        public:
            game(sigmafive::engine *engine) : engine_(engine) {}

            virtual ~game() {}

            sigmafive::engine *engine();

            virtual void update() = 0;

            virtual void render(graphics::context *ctx) = 0;
        private:
            sigmafive::engine *engine_;
        };
    }
}

#pragma once

#include <sigmafive/config.hpp>
#include <sigmafive/game/game.hpp>

namespace sigmafive {
    namespace game {
        class SIGMAFIVE_API game_manager {
        public:
            void register_game(std::string name,std::unique_ptr<game_factory> factory) {
                factories[name] = factory;
            }
            void unregister_game(std::string name) {
                factories.erase(name);
            }
        private:
            std::unoreded_map<std::string,std::unique_ptr<game_factory>> factories;
        };
    }
}

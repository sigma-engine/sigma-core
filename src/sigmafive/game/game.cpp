#include <sigmafive/game/game.hpp>

#include <sigmafive/engine.hpp>

namespace sigmafive {
    namespace game {
        engine *game::engine() {
            return engine_;
        }
    }
}

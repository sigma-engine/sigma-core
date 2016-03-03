#ifndef SIGMAFIVE_ENGINE_GAME_HPP
#define SIGMAFIVE_ENGINE_GAME_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/transform.hpp>
#include <sigmafive/graphics/static_mesh.hpp>

#include <chrono>

namespace sigmafive {
    class SIGMAFIVE_API game {
    public:
        game(component_manager<transform> &transforms,
             component_manager<graphics::static_mesh> &static_meshes);

        virtual ~game();

        const component_manager<transform> &transforms() const;

        component_manager<transform> &transforms();

        component_manager<graphics::static_mesh> &static_meshes();

        const component_manager<graphics::static_mesh> &static_meshes() const;

        virtual void update(std::chrono::duration<float> dt) = 0;
    private:
        component_manager<transform> &transforms_;
        component_manager<graphics::static_mesh> &static_meshes_;
    };
}

#endif //SIGMAFIVE_ENGINE_GAME_HPP

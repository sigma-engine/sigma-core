#ifndef SIGMA_FIVE_FAKE_COMPONENTS_HPP
#define SIGMA_FIVE_FAKE_COMPONENTS_HPP

#include <sigmafive/game/component.hpp>
#include <sigmafive/game/component_registry.hpp>


class fake_1_component : public sigmafive::game::component {
    SIGMAFIVE_CLASS()
public:
private:
};
using fake_1_component_component_pool = sigmafive::game::simple_component_pool<fake_1_component>;
using fake_1_component_component_pool_factory = sigmafive::simple_factory<sigmafive::game::component_pool,fake_1_component_component_pool>;

class fake_2_component : public sigmafive::game::component {
    SIGMAFIVE_CLASS()
public:
private:
};
using fake_2_component_component_pool = sigmafive::game::simple_component_pool<fake_2_component>;
using fake_2_component_component_pool_factory = sigmafive::simple_factory<sigmafive::game::component_pool,fake_2_component_component_pool>;

#endif //SIGMA_FIVE_FAKE_COMPONENTS_HPP

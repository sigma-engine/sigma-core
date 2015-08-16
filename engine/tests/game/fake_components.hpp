#ifndef SIGMA_FIVE_FAKE_COMPONENTS_HPP
#define SIGMA_FIVE_FAKE_COMPONENTS_HPP

#include <game/component.hpp>
#include <game/component_pool.hpp>

class fake_1_component : public sigmafive::game::component {
    SIGMAFIVE_CLASS()
public:
    typedef sigmafive::game::component_pool<fake_1_component> pool_type;
private:
};

class fake_2_component : public sigmafive::game::component {
    SIGMAFIVE_CLASS()
public:
    typedef sigmafive::game::component_pool<fake_1_component> pool_type;
private:
};

#endif //SIGMA_FIVE_FAKE_COMPONENTS_HPP

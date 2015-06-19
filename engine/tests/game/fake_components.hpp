#ifndef SIGMA_FIVE_FAKE_COMPONENTS_HPP
#define SIGMA_FIVE_FAKE_COMPONENTS_HPP

#include <game/component.hpp>
#include <game/component_pool.hpp>

class fake_1_component : public sigmafive::game::component {
    SIGMAFIVE_OBJECT()
public:
    typedef sigmafive::game::component_pool<fake_1_component> pool_type;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int format_version) {
        ar & SIGMAFIVE_SERIALIZE_BASE(sigmafive::game::component);
    }
private:
};

class fake_2_component : public sigmafive::game::component {
    SIGMAFIVE_OBJECT()
public:
    typedef sigmafive::game::component_pool<fake_1_component> pool_type;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int format_version) {
        ar & SIGMAFIVE_SERIALIZE_BASE(sigmafive::game::component);
    }
private:
};

#endif //SIGMA_FIVE_FAKE_COMPONENTS_HPP

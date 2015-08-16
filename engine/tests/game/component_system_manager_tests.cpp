#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <game/entity_manager.hpp>
#include <game/component_system_manager.hpp>

class fake_1_component_system : public sigmafive::game::component_system {
    SIGMAFIVE_CLASS()
public:
    virtual ~fake_1_component_system() {
    }

    virtual bool is_intrested(sigmafive::game::bitset mask) const override {
        return false;
    }

    virtual void entity_added(sigmafive::game::entity e) override {

    }

    virtual void entity_removed(sigmafive::game::entity e) override {

    }

    virtual void process() override {

    }
};
EXPORT_SIGMAFIVE_CLASS(fake_1_component_system)

TEST(component_system_manager_tests,add_component_system) {
    sigmafive::game::component_system_manager component_system_manager;

    auto component_system = component_system_manager.add_component_system<fake_1_component_system>();

    EXPECT_NE(nullptr, component_system);
}

TEST(component_system_manager_tests,has_component_system_false) {
    sigmafive::game::component_system_manager component_system_manager;

    EXPECT_FALSE(component_system_manager.has_component_system<fake_1_component_system>());
}

TEST(component_system_manager_tests,has_component_system_true) {
    sigmafive::game::component_system_manager component_system_manager;
    component_system_manager.add_component_system<fake_1_component_system>();

    EXPECT_TRUE(component_system_manager.has_component_system<fake_1_component_system>());
}


TEST(component_system_manager_tests,get_component_system) {
    sigmafive::game::component_system_manager component_system_manager;
    auto component_system = component_system_manager.add_component_system<fake_1_component_system>();

    EXPECT_EQ(component_system,component_system_manager.get_component_system<fake_1_component_system>());
}

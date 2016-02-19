#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/entity/entity_manager.hpp>
#include <sigmafive/entity/component_system.hpp>
#include <sigmafive/entity/component_manager.hpp>
#include <sigmafive/entity/component_system_manager.hpp>

class fake_1_component_system : public sigmafive::entity::component_system {
CPPBR_META_CLASS()
public:
    virtual ~fake_1_component_system() {
    }

    virtual void process(sigmafive::entity::entity_manager *em, sigmafive::entity::component_manager *cm) override {

    }
};

EXPORT_CPPBR_META_CLASS(fake_1_component_system)

TEST(component_system_manager_tests, add_component_system) {
    sigmafive::entity::component_system_manager component_system_manager;

    auto component_system = component_system_manager.add_component_system<fake_1_component_system>();

    EXPECT_NE(nullptr, component_system);
}

TEST(component_system_manager_tests, has_component_system_false) {
    sigmafive::entity::component_system_manager component_system_manager;

    EXPECT_FALSE(component_system_manager.has_component_system<fake_1_component_system>());
}

TEST(component_system_manager_tests, has_component_system_true) {
    sigmafive::entity::component_system_manager component_system_manager;
    component_system_manager.add_component_system<fake_1_component_system>();

    EXPECT_TRUE(component_system_manager.has_component_system<fake_1_component_system>());
}


TEST(component_system_manager_tests, get_component_system) {
    sigmafive::entity::component_system_manager component_system_manager;
    auto component_system = component_system_manager.add_component_system<fake_1_component_system>();

    EXPECT_EQ(component_system, component_system_manager.get_component_system<fake_1_component_system>());
}

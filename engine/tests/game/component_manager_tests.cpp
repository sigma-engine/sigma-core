#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <game/fake_components.hpp>

#include <sigmafive/game/entity_manager.hpp>
#include <sigmafive/game/component_manager.hpp>

class component_manager_tests : public ::testing::Test {
protected:
    component_manager_tests() : component_manager(&registry) { }

    virtual void SetUp() override {
        registry.register_component(fake_1_component::CLASS_ID,
                                    std::unique_ptr<fake_1_component_component_pool_factory>(
                                            new fake_1_component_component_pool_factory{}));
        registry.register_component(fake_2_component::CLASS_ID,
                                    std::unique_ptr<fake_2_component_component_pool_factory>(
                                            new fake_2_component_component_pool_factory{}));
    }

    sigmafive::game::component_registry registry;
    sigmafive::game::component_manager component_manager;
    sigmafive::game::entity_manager entity_manager;
};

TEST_F(component_manager_tests, add_component_T) {
    auto component = component_manager.add_component<fake_1_component>(entity_manager.create());

    EXPECT_NE(nullptr, component);
}

TEST_F(component_manager_tests, get_component_T_gets_same_component_that_was_created) {
    auto e = entity_manager.create();
    auto created_component = component_manager.add_component<fake_1_component>(e);

    auto got_component = component_manager.get_component<fake_1_component>(e);

    EXPECT_EQ(created_component, got_component);
}

TEST_F(component_manager_tests, has_component_false) {
    auto e = entity_manager.create();

    EXPECT_FALSE(component_manager.has_component<fake_1_component>(e));
}

TEST_F(component_manager_tests, has_component_true) {
    auto e = entity_manager.create();
    component_manager.add_component<fake_1_component>(e);

    EXPECT_TRUE(component_manager.has_component<fake_1_component>(e));
}

TEST_F(component_manager_tests, get_component_mask_none) {
    auto e = entity_manager.create();

    auto mask = component_manager.get_component_mask(e);

    EXPECT_FALSE(mask.any());
}

TEST_F(component_manager_tests, get_component_mask_one) {
    auto e = entity_manager.create();

    component_manager.add_component<fake_1_component>(e);

    auto mask = component_manager.get_component_mask(e);

    EXPECT_EQ(mask, registry.mask_for(fake_1_component::CLASS_ID));
}

TEST_F(component_manager_tests, get_component_mask_two) {
    auto e = entity_manager.create();

    component_manager.add_component<fake_1_component>(e);
    component_manager.add_component<fake_2_component>(e);

    auto mask = component_manager.get_component_mask(e);

    EXPECT_EQ(mask, registry.mask_for(fake_1_component::CLASS_ID) |
                    registry.mask_for(fake_2_component::CLASS_ID));
}

TEST_F(component_manager_tests, remove_component) {
    auto e = entity_manager.create();

    component_manager.add_component<fake_1_component>(e);
    component_manager.add_component<fake_2_component>(e);

    component_manager.remove_component<fake_2_component>(e);

    EXPECT_TRUE(component_manager.has_component<fake_1_component>(e));
    EXPECT_FALSE(component_manager.has_component<fake_2_component>(e));
}

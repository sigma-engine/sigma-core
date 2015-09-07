#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/game/entity_world.hpp>
class entity_world_tests : public ::testing::Test {
public:
    entity_world_tests() : entity_world(registry) {}
    virtual void SetUp() override {
    }

    sigmafive::game::component_registry registry;
    sigmafive::game::entity_world entity_world;
};

TEST_F(entity_world_tests,default_constructor) {
    (void)entity_world;
}

TEST_F(entity_world_tests,create) {
    auto e = entity_world.create();
    EXPECT_TRUE(entity_world.is_alive(e));
}

TEST_F(entity_world_tests,is_alive_true) {
    auto e = entity_world.create();
    EXPECT_TRUE(entity_world.is_alive(e));
}

TEST_F(entity_world_tests,is_alive_and_destroy_false) {
    auto e = entity_world.create();
    entity_world.destroy(e);
    EXPECT_FALSE(entity_world.is_alive(e));
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/game/entity_world.hpp>

TEST(entity_world_tests,default_constructor) {
    sigmafive::game::entity_world entity_world;
    (void)entity_world;
}

TEST(entity_world_tests,create) {
    sigmafive::game::entity_world entity_world;
    auto e = entity_world.create();
    EXPECT_TRUE(entity_world.is_alive(e));
}

TEST(entity_world_tests,is_alive_true) {
    sigmafive::game::entity_world entity_world;
    auto e = entity_world.create();
    EXPECT_TRUE(entity_world.is_alive(e));
}

TEST(entity_world_tests,is_alive_and_destroy_false) {
    sigmafive::game::entity_world entity_world;
    auto e = entity_world.create();
    entity_world.destroy(e);
    EXPECT_FALSE(entity_world.is_alive(e));
}

/*TEST(entity_world_tests,add_component) {
    sigmafive::game::entity_world entity_world;
    auto e = entity_world.create();

    entity_world.add_component<fake_1_component>(e);

    EXPECT_TRUE(entity_world.has_component<fake_1_component>(e));
}*/
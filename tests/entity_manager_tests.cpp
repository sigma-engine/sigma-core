#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <sigma/entity_manager.hpp>

TEST(entity_manager_tests, default_constructor)
{
    sigma::entity_manager entity_manager;
    (void)entity_manager;
}

TEST(entity_manager_tests, create_is_valid)
{
    sigma::entity_manager entity_manager;
    auto e = entity_manager.create();
    EXPECT_TRUE(e.is_valid());
}

TEST(entity_manager_tests, create_is_valid_and_new)
{
    sigma::entity_manager entity_manager;
    auto e1 = entity_manager.create();
    auto e2 = entity_manager.create();
    EXPECT_NE(e1, e2);
}

TEST(entity_manager_tests, is_alive_false)
{
    sigma::entity_manager entity_manager;
    sigma::entity e{ 1, 8 };
    EXPECT_FALSE(entity_manager.is_alive(e));
}

TEST(entity_manager_tests, is_alive_true)
{
    sigma::entity_manager entity_manager;
    auto e = entity_manager.create();
    EXPECT_TRUE(entity_manager.is_alive(e));
}

TEST(entity_manager_tests, destroy_not_alive)
{
    sigma::entity_manager entity_manager;
    auto e = entity_manager.create();
    entity_manager.destroy(e);
    EXPECT_FALSE(entity_manager.is_alive(e));
}

TEST(entity_manager_tests, create_from_destroyed_must_have_same_index)
{
    sigma::entity_manager entity_manager;
    auto e1 = entity_manager.create();
    entity_manager.destroy(e1);
    auto e2 = entity_manager.create();
    EXPECT_EQ(e1.index, e2.index);
}

TEST(entity_manager_tests, create_from_destroyed_must_have_new_version)
{
    sigma::entity_manager entity_manager;
    auto e1 = entity_manager.create();
    entity_manager.destroy(e1);
    auto e2 = entity_manager.create();
    EXPECT_NE(e1, e2);
}

TEST(entity_manager_tests, iterate_over_alive_entities)
{
    sigma::entity_manager entity_manager;
    std::vector<sigma::entity> created_entities;
    for (int i = 0; i < 50; ++i) {
        created_entities.push_back(entity_manager.create());
    }
    std::vector<sigma::entity> still_alive;
    for (auto e : created_entities) {
        if (e.index % 7 == 0)
            entity_manager.destroy(e);
        else
            still_alive.push_back(e);
    }

    auto it = still_alive.begin();
    auto end = still_alive.end();
    for (auto e : entity_manager) {
        EXPECT_NE(it, end);
        EXPECT_EQ(*it, e);
        ++it;
    }
}

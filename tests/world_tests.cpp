#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unordered_map>

#include "mock_components.hpp"

TEST(world_tests, create_is_valid)
{
    test_world w;

    auto e = w.create();

    EXPECT_TRUE(e.is_valid());
}

TEST(world_tests, create_is_valid_and_new)
{
    test_world w;

    auto e1 = w.create();
    auto e2 = w.create();

    EXPECT_NE(e1, e2);
}

TEST(world_tests, is_alive_false)
{
    test_world w;

    sigma::entity e{ 1, 8 };

    EXPECT_FALSE(w.is_alive(e));
}

TEST(world_tests, is_alive_true)
{
    test_world w;

    auto e = w.create();

    EXPECT_TRUE(w.is_alive(e));
}

TEST(world_tests, destroy_not_alive)
{
    test_world w;
    auto e = w.create();

    w.destroy(e);

    EXPECT_FALSE(w.is_alive(e));
}

TEST(world_tests, create_from_destroyed_must_have_same_index_but_new_version)
{
    test_world w;
    auto e1 = w.create();
    w.destroy(e1);

    auto e2 = w.create();

    EXPECT_EQ(e1.index, e2.index);
    EXPECT_NE(e1, e2);
}

TEST(world_tests, add_component_is_added_to_entity)
{
    test_world w;
    auto e = w.create();

    auto c = w.add<construction_component>(e, 12, 894);

    EXPECT_TRUE(w.has<construction_component>(e));
}

TEST(world_tests, add_component_was_created)
{
    test_world w;
    auto e = w.create();

    auto c = w.add<construction_component>(e, 12, 894);

    ASSERT_NE(nullptr, c);
    EXPECT_EQ(12, c->x);
    EXPECT_EQ(894, c->y);
}

TEST(world_tests, getting_component_should_return_the_same_value_returned_by_add)
{
    test_world w;
    auto e = w.create();
    auto cmp = w.add<construction_component>(e, 12, 894);

    EXPECT_EQ(cmp, w.get<construction_component>(e));
}

TEST(world_tests, adding_component_should_not_invalidate_other_entities_components_of_the_same_type)
{
    test_world w;

    auto test_e = w.create();
    auto test_cmp = w.add<construction_component>(test_e, 56, 232);

    std::vector<std::pair<sigma::entity, construction_component*>> components;
    for (int i = 0; i < 128; ++i) {
        auto e = w.create();
        auto c = w.add<construction_component>(e, 23, 423);
        components.push_back(std::make_pair(e, c));
    }

    for (auto comp : components)
        EXPECT_EQ(comp.second, w.get<construction_component>(comp.first));
}

TEST(world_tests, remove_only_destruction_component_while_calling_destructor)
{
    test_world w;
    auto e = w.create();
    w.add<construction_component>(e, 12, 894);
    auto c = w.add<destruction_component>(e, 12, 894);

    EXPECT_CALL(*c, destructor())
        .Times(1);

    w.remove<destruction_component>(e);

    EXPECT_FALSE(w.has<destruction_component>(e));
    EXPECT_TRUE(w.has<construction_component>(e));
}

TEST(world_tests, destroy_removes_all_components_and_calls_their_destructors)
{
    test_world w;
    auto e = w.create();
    w.add<construction_component>(e, 12, 894);
    auto c = w.add<destruction_component>(e, 12, 894);

    EXPECT_CALL(*c, destructor())
        .Times(1);

    w.destroy(e);

    EXPECT_FALSE(w.has<destruction_component>(e));
    EXPECT_FALSE(w.has<construction_component>(e));
}

TEST(world_tests, for_each_10_entites_with_two_components_10_with_one)
{
    test_world w;
    std::unordered_map<sigma::entity, int> counters;
    for (int i = 0; i < 20; ++i) {
        auto e = w.create();
        w.add<basic_component>(e, 170, 286);
        counters[e] = 0;
        if (i % 2 == 0) {
            w.add<construction_component>(e, 8, 9);
            counters[e] = 0;
        }
    }

    int count = 0;
    w.for_each<basic_component>([&](auto e, const auto& c1) {
        EXPECT_TRUE(w.has<basic_component>(e));

        EXPECT_EQ(170, c1.x);
        EXPECT_EQ(286, c1.y);

        ASSERT_EQ(1, counters.count(e));
        EXPECT_EQ(0, counters[e]) << "meaning for_each looped over entity more than once";
        counters[e] += 1;
        count++;
    });
    EXPECT_EQ(20, count);

    count = 0;
    w.for_each<construction_component>([&](auto e, const auto& c2) {
        EXPECT_TRUE(w.has<construction_component>(e));

        EXPECT_EQ(8, c2.x);
        EXPECT_EQ(9, c2.y);

        ASSERT_EQ(1, counters.count(e));
        EXPECT_EQ(1, counters[e]) << "meaning for_each looped over entity more than once";
        counters[e] += 1;
        count++;
    });
    EXPECT_EQ(10, count);

    count = 0;
    w.for_each<basic_component, construction_component>([&](auto e, const auto& c1, const auto& c2) {
        EXPECT_TRUE(w.has<basic_component>(e));
        EXPECT_TRUE(w.has<construction_component>(e));

        EXPECT_EQ(170, c1.x);
        EXPECT_EQ(286, c1.y);

        EXPECT_EQ(8, c2.x);
        EXPECT_EQ(9, c2.y);

        ASSERT_EQ(1, counters.count(e));
        EXPECT_EQ(2, counters[e]) << "meaning for_each looped over entity more than once";
        counters[e] += 1;
        count++;
    });
    EXPECT_EQ(10, count);
}

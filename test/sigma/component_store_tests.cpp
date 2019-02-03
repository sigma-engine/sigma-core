#include <sigma/component_store.hpp>

#include "mock_components.hpp"

#include <gtest/gtest.h>

TEST(component_store, add_before_commit_should_have_not_effect)
{
    sigma::component_store<basic_component> store;

    store.add(7);
    EXPECT_EQ(store.get(7), nullptr);
}

TEST(component_store, add_after_commit_should_create_component)
{
    sigma::component_store<basic_component> store;

    store.add(7);
    store.commit();
    EXPECT_NE(nullptr, store.get(7));
}

TEST(component_store, remove_before_commit_should_have_not_effect)
{
    sigma::component_store<basic_component> store;
    store.add(7);
    store.commit();

    store.remove(7);
    EXPECT_NE(store.get(7), nullptr);
}

TEST(component_store, remove_after_commit_should_remove_component)
{
    sigma::component_store<basic_component> store;
    store.add(7);
    store.commit();

    store.remove(7);
    store.commit();
    EXPECT_EQ(nullptr, store.get(7));
}

TEST(component_store, get_multiple_calls_should_be_the_same)
{
    sigma::component_store<basic_component> store;
    store.add(7);
    store.commit();

    auto a = store.get(7);
    auto b = store.get(7);
    EXPECT_EQ(a, b);
}

TEST(component_store, add_multiple_before_commit_should_not_invalid)
{
    sigma::component_store<basic_component> store;
    std::vector<basic_component*> stored;
    for (int e = 0; e < 50; ++e)
        store.add(e);
    store.commit();
    for (int e = 0; e < 50; ++e)
        stored.push_back(store.get(e));

    for (int e = 50; e < 100; ++e)
        store.add(e);
    for (int e = 0; e < 50; ++e)
        EXPECT_EQ(stored[e], store.get(e));
}

TEST(component_store, remove_multiple_before_commit_should_not_invalid)
{
    sigma::component_store<basic_component> store;
    std::vector<basic_component*> stored;
    for (int e = 0; e < 50; ++e)
        store.add(e);
    store.commit();
    for (int e = 0; e < 50; ++e)
        stored.push_back(store.get(e));

    for (int e = 50; e < 100; ++e)
        store.remove(e);
    for (int e = 0; e < 50; ++e)
        EXPECT_EQ(stored[e], store.get(e));
}

TEST(component_store, commit_more_removed_than_added_should_not_crash)
{
    sigma::component_store<basic_component> store;
    for (int e = 0; e < 50; ++e)
        store.add(e);
    store.commit();

    for (int e = 0; e < 25; ++e)
        store.remove(e);

    for (int e = 50; e < 55; ++e)
        store.add(e);

    store.commit();

    SUCCEED();
}

TEST(component_store, commit_more_added_than_removed_should_not_crash)
{
    sigma::component_store<basic_component> store;
    for (int e = 0; e < 50; ++e)
        store.add(e);
    store.commit();

    for (int e = 50; e < 75; ++e)
        store.add(e);

    for (int e = 0; e < 5; ++e)
        store.remove(e);

    store.commit();

    SUCCEED();
}

TEST(component_store, commit_remove_add_some_entity_should_not_remove_again)
{
    sigma::component_store<basic_component> store;
    store.add(7);
    store.commit();

    store.remove(7);
    store.commit();

    store.add(7);
    store.commit();

    EXPECT_NE(nullptr, store.get(7));
}

TEST(component_store, commit_add_remove_some_entity_should_not_add_again)
{
    sigma::component_store<basic_component> store;
    store.add(7);
    store.commit();

    store.remove(7);
    store.commit();

    EXPECT_EQ(nullptr, store.get(7));
}

TEST(component_store, join_test)
{
    sigma::component_store<basic_component> basic_store;
    sigma::component_store<simple_component> simple_store;
    basic_store.add(1);
    basic_store.commit();

    simple_store.add(1);
    simple_store.add(2);
    simple_store.commit();

    auto dataset = basic_store.join(simple_store);
    EXPECT_EQ(1, dataset.size());
}

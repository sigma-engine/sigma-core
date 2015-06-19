#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <game/fake_components.hpp>

#include <game/entity_manager.hpp>
#include <game/component_manager.hpp>

TEST(component_manager_tests,add_component_T) {
    sigmafive::game::component_manager component_manager;
    sigmafive::game::entity_manager entity_manager;

    auto component = component_manager.add_component<fake_1_component>(entity_manager.create());

    EXPECT_NE(nullptr,component);
}

TEST(component_manager_tests,get_component_T_gets_same_component_that_was_created) {
    sigmafive::game::component_manager component_manager;
    sigmafive::game::entity_manager entity_manager;
    auto e = entity_manager.create();
    auto created_component = component_manager.add_component<fake_1_component>(e);

    auto got_component = component_manager.get_component<fake_1_component>(e);

    EXPECT_EQ(created_component, got_component);
}

TEST(component_manager_tests,has_component_false) {
    sigmafive::game::component_manager component_manager;
    sigmafive::game::entity_manager entity_manager;
    auto e = entity_manager.create();

    EXPECT_FALSE(component_manager.has_component<fake_1_component>(e));
}

TEST(component_manager_tests,has_component_true) {
    sigmafive::game::component_manager component_manager;
    sigmafive::game::entity_manager entity_manager;
    auto e = entity_manager.create();
    component_manager.add_component<fake_1_component>(e);

    EXPECT_TRUE(component_manager.has_component<fake_1_component>(e));
}

TEST(component_manager_tests,get_component_mask_none) {
    sigmafive::game::component_manager component_manager;
    sigmafive::game::entity_manager entity_manager;
    auto e = entity_manager.create();

    auto mask = component_manager.get_component_mask(e);

    EXPECT_FALSE(mask.any());
}

TEST(component_manager_tests,get_component_mask_one) {
    sigmafive::game::component_manager component_manager;
    sigmafive::game::entity_manager entity_manager;
    auto e = entity_manager.create();

    component_manager.add_component<fake_1_component>(e);

    auto mask = component_manager.get_component_mask(e);

    EXPECT_EQ(mask,component_manager.bitset_manager().bitset_for(fake_1_component::CLASS_ID));
}

TEST(component_manager_tests,get_component_mask_two) {
    sigmafive::game::component_manager component_manager;
    sigmafive::game::entity_manager entity_manager;
    auto e = entity_manager.create();

    component_manager.add_component<fake_1_component>(e);
    component_manager.add_component<fake_2_component>(e);

    auto mask = component_manager.get_component_mask(e);

    EXPECT_EQ(mask,component_manager.bitset_manager().bitset_for(fake_1_component::CLASS_ID)|
                   component_manager.bitset_manager().bitset_for(fake_2_component::CLASS_ID));
}

TEST(component_manager_tests,remove_component) {
    sigmafive::game::component_manager component_manager;
    sigmafive::game::entity_manager entity_manager;
    auto e = entity_manager.create();

    component_manager.add_component<fake_1_component>(e);
    component_manager.add_component<fake_2_component>(e);

    component_manager.remove_component<fake_2_component>(e);

    EXPECT_TRUE(component_manager.has_component<fake_1_component>(e));
    EXPECT_FALSE(component_manager.has_component<fake_2_component>(e));
}

/*TEST(component_manager_tests,serialize) {
    sigmafive::game::component_manager component_manager1;
    sigmafive::game::component_manager component_manager2;
    sigmafive::game::entity_manager entity_manager;
    auto e1 = entity_manager.create();
    auto e2 = entity_manager.create();
    auto e3 = entity_manager.create();

    component_manager1.add_component<fake_1_component>(e1);
    component_manager1.add_component<fake_2_component>(e1);

    component_manager1.add_component<fake_1_component>(e2);

    std::stringstream ss;
    {
        boost::archive::text_oarchive oar(ss);
        oar << boost::serialization::make_nvp("component_manager",component_manager1);

    }
    {
        boost::archive::text_iarchive iar(ss);
        iar >> boost::serialization::make_nvp("component_manager",component_manager2);
    }

    EXPECT_TRUE(component_manager2.has_component<fake_1_component>(e1));
    EXPECT_NE(nullptr,component_manager2.get_component<fake_1_component>(e1));
    EXPECT_TRUE(component_manager2.has_component<fake_2_component>(e1));
    EXPECT_NE(nullptr,component_manager2.get_component<fake_2_component>(e1));

    EXPECT_TRUE(component_manager2.has_component<fake_1_component>(e2));
    EXPECT_NE(nullptr,component_manager2.get_component<fake_1_component>(e2));
    EXPECT_FALSE(component_manager2.has_component<fake_2_component>(e2));
    EXPECT_EQ(nullptr,component_manager2.get_component<fake_2_component>(e2));

    EXPECT_FALSE(component_manager2.has_component<fake_1_component>(e3));
    EXPECT_EQ(nullptr,component_manager2.get_component<fake_1_component>(e3));
    EXPECT_FALSE(component_manager2.has_component<fake_2_component>(e3));
    EXPECT_EQ(nullptr,component_manager2.get_component<fake_2_component>(e3));
}*/

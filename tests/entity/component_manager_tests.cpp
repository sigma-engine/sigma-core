#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <entity/fake_components.hpp>
#include <sigmafive/entity/component_manager.hpp>
#include <sigmafive/entity/default_component_cache.hpp>
#include <sigmafive/entity/component_cache_factory.hpp>

using namespace sigmafive::entity;

class component_manager_tests : public ::testing::Test {
public:
    component_manager_tests() : cm(cr) {
        cr.register_cache<c2>(default_component_cache<c1>::factory);
        cr.register_cache<c1>(default_component_cache<c2>::factory);
    }
protected:
    component_cache_factory cr;
    component_manager cm;
};

TEST_F(component_manager_tests,entity_does_not_have_component_has_component_returns_false_and_get_component_is_nullptr) {
    EXPECT_FALSE(cm.has_component<c1>(entity{0,0}));
    EXPECT_EQ(nullptr,cm.get_component<c1>(entity{0,0}));
}

TEST_F(component_manager_tests,entity_has_component_has_component_returns_true_and_get_component_is_not_nullptr) {
    entity e{0,0};

    cm.add_component<c1>(e);

    EXPECT_TRUE(cm.has_component<c1>(e));
    EXPECT_NE(nullptr,cm.get_component<c1>(e));
}

TEST_F(component_manager_tests,has_component_component_has_been_removed_from_entity_return_false) {
    entity e{0,0};
    cm.add_component<c1>(e);

    cm.remove_component<c1>(e);

    EXPECT_FALSE(cm.has_component<c1>(e));
}

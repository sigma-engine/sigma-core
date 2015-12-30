#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/game/component.hpp>

TEST(component_tests, component_is_polymorphic_true) {
    EXPECT_TRUE(std::is_polymorphic<sigmafive::game::component>::value);
}

TEST(component_tests, component_has_ID) {
    auto e = sigmafive::game::component::CLASS_ID;
    (void) e;
}

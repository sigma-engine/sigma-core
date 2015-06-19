#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <game/component_system.hpp>

TEST(component_system_tests,component_system_is_polymorphic_true) {
    EXPECT_TRUE(std::is_polymorphic<sigmafive::game::component_system>::value);
}

TEST(component_system_tests,component_system_has_ID) {
    auto s = sigmafive::game::component_system::CLASS_ID;
    (void)s;
}

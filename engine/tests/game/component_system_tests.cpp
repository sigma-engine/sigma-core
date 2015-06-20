#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <game/component_system.hpp>

TEST(component_system_tests,CLASS) {
    EXPECT_EQ(std::string("sigmafive::game::component_system"),sigmafive::game::component_system::CLASS);
}

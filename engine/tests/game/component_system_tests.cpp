#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/game/component_system.hpp>

TEST(component_system_tests,CLASS_ID) {
    EXPECT_EQ(sigmafive::compile_time_hash("sigmafive::game::component_system"),sigmafive::game::component_system::CLASS_ID);
}

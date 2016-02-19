#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/entity/component_system.hpp>

TEST(component_system_tests, CLASS_ID) {
    EXPECT_EQ(cppbr::meta::compile_time_hash("sigmafive::entity::component_system"),
              sigmafive::entity::component_system::CLASS_ID);
}

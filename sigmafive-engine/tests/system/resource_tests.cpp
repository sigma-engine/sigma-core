#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/system/resource.hpp>

TEST(resource_tests,CLASS_ID) {
    EXPECT_EQ(sigmafive::compile_time_hash("sigmafive::system::resource"),sigmafive::system::resource::CLASS_ID);
}

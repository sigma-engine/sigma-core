#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/resource/resource.hpp>

TEST(resource_tests, CLASS_ID) {
    EXPECT_EQ(sigmafive::compile_time_hash("sigmafive::resource::resource"), sigmafive::resource::resource::CLASS_ID);
}

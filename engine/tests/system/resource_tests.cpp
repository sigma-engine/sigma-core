#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <system/resource.hpp>

TEST(resource_tests,CLASS) {
    EXPECT_EQ(std::string("sigmafive::system::resource"),sigmafive::system::resource::CLASS);
}

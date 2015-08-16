#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sigmafive/math/utils.hpp>

TEST(util_tests,almost_equal_real_true) {
	float a = 1.1f;
	float b = 1.1f;
	EXPECT_TRUE(almost_equal(a,b));
}

TEST(util_tests,almost_equal_false) {
	float a = 1.1f;
	float b = 1.12f;
	EXPECT_FALSE(almost_equal(a,b));
}

TEST(util_tests,almost_equal_integer_true) {
	int a = 1;
	int b = 1;
	EXPECT_TRUE(almost_equal(a,b));
}

TEST(util_tests,almost_equal_integer_false) {
	int a = 1;
	int b = 2;
	EXPECT_FALSE(almost_equal(a,b));
}

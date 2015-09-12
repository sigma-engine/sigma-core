#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/vec3.hpp>
#include <sigmafive/math/vec4.hpp>


TEST(vec2_tests,default_constructor) {
	int2 a;
	EXPECT_EQ(int2(0,0),a);
}

TEST(vec2_tests,constructor_two_values) {
	int2 a(1,2);
	EXPECT_EQ(int2(1,2),a);
}

TEST(vec2_tests,constructor_vec3) {
	int2 a(int3(1,2,3));
	EXPECT_EQ(int2(1,2),a);
}

TEST(vec2_tests,constructor_vec4) {
	int2 a(int4(1,2,3,4));
	EXPECT_EQ(int2(1,2),a);
}

TEST(vec2_tests,equal_true) {
	int2 a(1,5);
	int2 b(1,5);
	EXPECT_TRUE(a==b);
}

TEST(vec2_tests,equal_false) {
	int2 a(1,7);
	int2 b(1,5);
	EXPECT_FALSE(a==b);
}

TEST(vec2_tests,not_equal_false) {
	int2 a(1,5);
	int2 b(1,5);
	EXPECT_FALSE(a!=b);
}

TEST(vec2_tests,not_equal_true) {
	int2 a(1,7);
	int2 b(1,5);
	EXPECT_TRUE(a!=b);
}

TEST(vec2_tests,less_than_true) {
	int2 a(1,5);
	int2 b(2,7);
	EXPECT_TRUE(a<b);
}

TEST(vec2_tests,less_than_false) {
	int2 a(2,7);
	int2 b(1,5);
	EXPECT_FALSE(a<b);
}

TEST(vec2_tests,less_than_equal_true) {
	int2 a(1,5);
	int2 b(2,7);
	EXPECT_TRUE(a<=b);
}

TEST(vec2_tests,less_than_equal_false) {
	int2 a(2,7);
	int2 b(1,5);
	EXPECT_FALSE(a<=b);
}

TEST(vec2_tests,greater_than_true) {
	int2 a(2,7);
	int2 b(1,5);
	EXPECT_TRUE(a>b);
}

TEST(vec2_tests,greater_than_false) {
	int2 a(1,5);
	int2 b(2,7);
	EXPECT_FALSE(a>b);
}

TEST(vec2_tests,greater_than_equal_true) {
	int2 a(2,7);
	int2 b(1,5);
	EXPECT_TRUE(a>=b);
}

TEST(vec2_tests,greater_than_equal_false) {
	int2 a(1,5);
	int2 b(2,7);
	EXPECT_FALSE(a>=b);
}

TEST(vec2_tests,add) {
	int2 a(1,5);
	int2 b(2,7);
	EXPECT_EQ(int2(3,12),a+b);
}

TEST(vec2_tests,sub) {
	int2 a(1,5);
	int2 b(2,7);
	EXPECT_EQ(int2(-1,-2),a-b);
}

TEST(vec2_tests,neg) {
	int2 a(1,5);
	EXPECT_EQ(int2(-1,-5),-a);
}

TEST(vec2_tests,mult_scaler) {
	int2 a(1,5);
	int s = 15;
	EXPECT_EQ(int2(15,75),a*s);
}

TEST(vec2_tests,div_scaler) {
	int2 a(15,75);
	int s = 15;
	EXPECT_EQ(int2(1,5),a/s);
}

TEST(vec2_tests,add_assign) {
	int2 a(1,5);
	a += int2(2,7);
	EXPECT_EQ(int2(3,12),a);
}

TEST(vec2_tests,sub_assign) {
	int2 a(1,5);
	a -= int2(2,7);
	EXPECT_EQ(int2(-1,-2),a);
}

TEST(vec2_tests,mult_assign_scaler) {
	int2 a(1,5);
	a *= 15;
	EXPECT_EQ(int2(15,75),a);
}

TEST(vec2_tests,div_assign_scaler) {
	int2 a(15,75);
	a /= 15;
	EXPECT_EQ(int2(1,5),a);
}

TEST(vec2_tests,length) {
	float2 a(1,5);
	EXPECT_FLOAT_EQ(std::sqrt(26.0f),a.length());
}

TEST(vec2_tests,square_length) {
	float2 a(1,5);
	EXPECT_FLOAT_EQ(26.0f, a.square_length());
}

TEST(vec2_tests,get_normalized) {
	float2 a(1,5);
	float2 result = a.get_normalized();
	EXPECT_EQ(a/std::sqrt(26.0f),result);
}

TEST(vec2_tests,dot) {
	float2 a(1,5);
	float2 b(1,7);

	EXPECT_FLOAT_EQ(36.0f,a.dot(b));
}

TEST(vec2_tests,normalize) {
	float2 a(1,5);
	float2 expected = a/std::sqrt(26.0f);

	a.normalize();
	EXPECT_EQ(expected,a);
}


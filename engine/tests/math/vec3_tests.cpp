#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/vec3.hpp>
#include <sigmafive/math/vec4.hpp>

TEST(vec3_tests,default_constructor) {
	int3 a;
	EXPECT_EQ(int3(0,0,0),a);
}

TEST(vec3_tests,constructor_three_values) {
	int3 a(1,2,3);
	EXPECT_EQ(int3(1,2,3),a);
}

TEST(vec3_tests,constructor_vec2_and_value) {
	int3 a(int2(1,2),3);
	EXPECT_EQ(int3(1,2,3),a);
}

TEST(vec3_tests,constructor_vec4) {
	int3 a(int4(1,2,3,4));
	EXPECT_EQ(int3(1,2,3),a);
}

TEST(vec3_tests,equal_true) {
	int3 a(1,5,1);
	int3 b(1,5,1);
	EXPECT_TRUE(a==b);
}

TEST(vec3_tests,equal_false) {
	int3 a(1,5,2);
	int3 b(1,5,1);
	EXPECT_FALSE(a==b);
}

TEST(vec3_tests,not_equal_false) {
	int3 a(1,5,1);
	int3 b(1,5,1);
	EXPECT_FALSE(a!=b);
}

TEST(vec3_tests,not_equal_true) {
	int3 a(1,5,2);
	int3 b(1,5,1);
	EXPECT_TRUE(a!=b);
}

TEST(vec3_tests,less_than_true) {
	int3 a(1,5,1);
	int3 b(2,7,43);
	EXPECT_TRUE(a<b);
}

TEST(vec3_tests,less_than_false) {
	int3 a(2,7,43);
	int3 b(1,5,1);
	EXPECT_FALSE(a<b);
}

TEST(vec3_tests,less_than_equal_true) {
	int3 a(1,5,1);
	int3 b(2,7,43);
	EXPECT_TRUE(a<=b);
}

TEST(vec3_tests,less_than_equal_false) {
	int3 a(2,7,43);
	int3 b(1,5,1);
	EXPECT_FALSE(a<=b);
}

TEST(vec3_tests,greater_than_true) {
	int3 a(2,7,43);
	int3 b(1,5,1);
	EXPECT_TRUE(a>b);
}

TEST(vec3_tests,greater_than_false) {
	int3 a(1,5,1);
	int3 b(2,7,43);
	EXPECT_FALSE(a>b);
}

TEST(vec3_tests,greater_than_equal_true) {
	int3 a(2,7,43);
	int3 b(1,5,1);
	EXPECT_TRUE(a>=b);
}

TEST(vec3_tests,greater_than_equal_false) {
	int3 a(1,5,1);
	int3 b(2,7,43);
	EXPECT_FALSE(a>=b);
}

TEST(vec3_tests,add) {
	int3 a(1,5,1);
	int3 b(2,7,43);
	EXPECT_EQ(int3(3,12,44),a+b);
}

TEST(vec3_tests,sub) {
	int3 a(1,5,1);
	int3 b(2,7,43);
	EXPECT_EQ(int3(-1,-2,-42),a-b);
}

TEST(vec3_tests,neg) {
	int3 a(1,5,1);
	EXPECT_EQ(int3(-1,-5,-1),-a);
}

TEST(vec3_tests,mult_scaler) {
	int3 a(1,5,1);
	int s = 15;
	EXPECT_EQ(int3(15,75,15),a*s);
}

TEST(vec3_tests,mult) {
	int3 a(1,5,1);
	int3 b(1,8,3);
	EXPECT_EQ(int3(7,-2,3),a*b);
}

TEST(vec3_tests,div_scaler) {
	int3 a(15,75,15);
	int s = 15;
	EXPECT_EQ(int3(1,5,1),a/s);
}

TEST(vec3_tests,add_assign) {
	int3 a(1,5,1);
	a += int3(2,7,43);
	EXPECT_EQ(int3(3,12,44),a);
}

TEST(vec3_tests,sub_assign) {
	int3 a(1,5,1);
	a -= int3(2,7,43);
	EXPECT_EQ(int3(-1,-2,-42),a);
}

TEST(vec3_tests,mult_assign_scaler) {
	int3 a(1,5,1);
	a *= 15;
	EXPECT_EQ(int3(15,75,15),a);
}

TEST(vec3_tests,div_assign_scaler) {
	int3 a(15,75,15);
	a /= 15;
	EXPECT_EQ(int3(1,5,1),a);
}

TEST(vec3_tests,length) {
	float3 a(1,5,1);
	EXPECT_FLOAT_EQ(std::sqrt(27.0f),a.length());
}

TEST(vec3_tests,square_length) {
	float3 a(1,5,1);
	EXPECT_FLOAT_EQ(27.0f, a.square_length());
}

TEST(vec3_tests,get_normalize) {
	float3 a(1,5,1);
	float3 result = a.get_normalized();
	EXPECT_EQ(a/std::sqrt(27.0f),result);
}

TEST(vec3_tests,dot) {
	float3 a(1,5,1);
	float3 b(1,7,3);

	EXPECT_FLOAT_EQ(39.0f,a.dot(b));
}

TEST(vec3_tests,normalize) {
	float3 a(1,5,1);
	float3 expected = a/std::sqrt(27.0f);

	a.normalize();
	EXPECT_EQ(expected,a);
}


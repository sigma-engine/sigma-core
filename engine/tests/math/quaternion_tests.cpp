#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/vec3.hpp>
#include <sigmafive/math/vec4.hpp>
#include <sigmafive/math/quaternion.hpp>

TEST(quaternion_tests,default_constructor) {
	quaternioni a;
	EXPECT_EQ(quaternioni(0,0,0,1),a);
}

TEST(quaternion_tests,constructor_four_values) {
	quaternioni a(1,2,3,4);
	EXPECT_EQ(quaternioni(1,2,3,4),a);
}

TEST(quaternion_tests,constructor_vec2_and_two_values) {
	quaternioni a(int2(1,2),3,4);
	EXPECT_EQ(quaternioni(1,2,3,4),a);
}

TEST(quaternion_tests,constructor_two_vec2s) {
	quaternioni a(int2(1,2),int2(3,4));
	EXPECT_EQ(quaternioni(1,2,3,4),a);
}

TEST(quaternion_tests,constructor_vec3_and_a_value) {
	quaternioni a(int3(1,2,3),4);
	EXPECT_EQ(quaternioni(1,2,3,4),a);
}

TEST(quaternion_tests,constructor_vec4) {
	quaternioni a(int4(1,2,3,4));
	EXPECT_EQ(quaternioni(1,2,3,4),a);
}

TEST(quaternion_tests,equal_true) {
	quaternioni a(1,5,1,2);
	quaternioni b(1,5,1,2);
	EXPECT_TRUE(a==b);
}

TEST(quaternion_tests,equal_false) {
	quaternioni a(1,5,1,2);
	quaternioni b(1,5,1,3);
	EXPECT_FALSE(a==b);
}

TEST(quaternion_tests,not_equal_false) {
	quaternioni a(1,5,1,2);
	quaternioni b(1,5,1,2);
	EXPECT_FALSE(a!=b);
}

TEST(quaternion_tests,not_equal_true) {
	quaternioni a(1,5,1,2);
	quaternioni b(1,5,1,3);
	EXPECT_TRUE(a!=b);
}

TEST(quaternion_tests,less_than_true) {
	quaternioni a(1,5,1,2);
	quaternioni b(2,7,43,55);
	EXPECT_TRUE(a<b);
}

TEST(quaternion_tests,less_than_false) {
	quaternioni a(2,7,43,55);
	quaternioni b(1,5,1,2);
	EXPECT_FALSE(a<b);
}

TEST(quaternion_tests,less_than_equal_true) {
	quaternioni a(1,5,1,2);
	quaternioni b(2,7,43,55);
	EXPECT_TRUE(a<=b);
}

TEST(quaternion_tests,less_than_equal_false) {
	quaternioni a(2,7,43,55);
	quaternioni b(1,5,1,2);
	EXPECT_FALSE(a<=b);
}

TEST(quaternion_tests,greater_than_true) {
	quaternioni a(2,7,43,55);
	quaternioni b(1,5,1,2);
	EXPECT_TRUE(a>b);
}

TEST(quaternion_tests,greater_than_false) {
	quaternioni a(1,5,1,2);
	quaternioni b(2,7,43,55);
	EXPECT_FALSE(a>b);
}

TEST(quaternion_tests,greater_than_equal_true) {
	quaternioni a(2,7,43,55);
	quaternioni b(1,5,1,2);
	EXPECT_TRUE(a>=b);
}

TEST(quaternion_tests,greater_than_equal_false) {
	quaternioni a(1,5,1,2);
	quaternioni b(2,7,43,55);
	EXPECT_FALSE(a>=b);
}

TEST(quaternion_tests,add) {
	quaternioni a(1,5,1,2);
	quaternioni b(2,7,43,55);
	EXPECT_EQ(quaternioni(3,12,44,57),a+b);
}

TEST(quaternion_tests,sub) {
	quaternioni a(1,5,1,2);
	quaternioni b(2,7,43,55);
	EXPECT_EQ(quaternioni(-1,-2,-42,-53),a-b);
}

TEST(quaternion_tests,neg) {
	quaternioni a(1,5,1,2);
	EXPECT_EQ(quaternioni(-1,-5,-1,-2),-a);
}

TEST(quaternion_tests,mult_quaternion) {
	quaternioni a(1,5,1,2);
	quaternioni b(1,7,6,7);
	EXPECT_EQ(quaternioni(32,44,21,-28),a*b);
}

TEST(quaternion_tests,mult_scaler) {
	quaternioni a(1,5,1,2);
	int s = 15;
	EXPECT_EQ(quaternioni(15,75,15,30),a*s);
}

TEST(quaternion_tests,div_scaler) {
	quaternioni a(15,75,15,30);
	int s = 15;
	EXPECT_EQ(quaternioni(1,5,1,2),a/s);
}

TEST(quaternion_tests,add_assign) {
	quaternioni a(1,5,1,2);
	a += quaternioni(2,7,43,55);
	EXPECT_EQ(quaternioni(3,12,44,57),a);
}

TEST(quaternion_tests,sub_assign) {
	quaternioni a(1,5,1,2);
	a -= quaternioni(2,7,43,55);
	EXPECT_EQ(quaternioni(-1,-2,-42,-53),a);
}

TEST(quaternion_tests,mult_assign_scaler) {
	quaternioni a(1,5,1,2);
	a *= 15;
	EXPECT_EQ(quaternioni(15,75,15,30),a);
}

TEST(quaternion_tests,div_assign_scaler) {
	quaternioni a(15,75,15,30);
	a /= 15;
	EXPECT_EQ(quaternioni(1,5,1,2),a);
}

TEST(quaternion_tests,length) {
	quaternionf a(1,5,1,2);
	EXPECT_FLOAT_EQ(std::sqrt(31.0f),a.length());
}

TEST(quaternion_tests,square_length) {
	quaternionf a(1,5,1,2);
	EXPECT_FLOAT_EQ(31.0f, a.square_length());
}

TEST(quaternion_tests,get_normalized) {
	quaternionf a(1,5,1,2);
	auto result = a.get_normalized();
	EXPECT_EQ(a/std::sqrt(31.0f),result);
}

TEST(quaternion_tests,get_conjugate) {
	quaternionf a(1,5,1,2);
	auto result = a.get_conjugate();
	EXPECT_EQ(quaternionf(-1,-5,-1,2),result);
}

TEST(quaternion_tests,get_inverse) {
	quaternionf a(1,5,1,2);
	auto result = a.get_inverse();
	EXPECT_EQ(quaternionf(-1,-5,-1,2)/31.0f,result);
}

TEST(quaternion_tests,dot_a_dot_b_43) {
	quaternionf a(1,5,1,2);
	quaternionf b(1,7,3,2);

	EXPECT_FLOAT_EQ(43.0f,a.dot(b));
}

TEST(quaternion_tests,normalize) {
	quaternionf a(1,5,1,2);
	quaternionf expected = a/std::sqrt(31.0f);

	a.normalize();
	EXPECT_EQ(expected,a);
}

TEST(quaternion_tests,conjugate) {
	quaternionf a(1,5,1,2);
	a.conjugate();
	EXPECT_EQ(quaternionf(-1,-5,-1,2),a);
}

TEST(quaternion_tests,inverse) {
	quaternionf a(1,5,1,2);
	a.inverse();
	EXPECT_EQ(quaternionf(-1,-5,-1,2)/31.0f,a);
}

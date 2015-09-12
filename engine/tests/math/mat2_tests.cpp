#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sigmafive/math/mat2.hpp>

TEST(mat2_tests,default_constructor) {
	int2x2 a;

	EXPECT_EQ(int2x2({1,0},
					{0,1}),a);
}

TEST(mat2_tests,constructor_two_vec2s) {
	int2x2 a({1,2},
			{1,3});
	EXPECT_EQ(int2x2({1,2},
					{1,3}),a);
}

TEST(mat2_tests,equal_true) {
	int2x2 a({1,2},
			{1,3});
	int2x2 b({1,2},
			{1,3});
	EXPECT_TRUE(a==b);
}

TEST(mat2_tests,equal_false) {
	int2x2 a({1,2},
			{1,3});
	int2x2 b({1,2},
			{4,3});
	EXPECT_FALSE(a==b);
}

TEST(mat2_tests,not_equal_false) {
	int2x2 a({1,2},
			{1,3});
	int2x2 b({1,2},
			{1,3});
	EXPECT_FALSE(a!=b);
}

TEST(mat2_tests,not_equal_true) {
	int2x2 a({1,2},
			{1,3});
	int2x2 b({1,2},
			{3,3});
	EXPECT_TRUE(a!=b);
}

TEST(mat2_tests,less_than_true) {
	int2x2 a({0,0},
			{0,0});
	int2x2 b({1,2},
			{1,3});
	EXPECT_TRUE(a<b);
}

TEST(mat2_tests,less_than_false) {
	int2x2 a({1,2},
			{1,3});
	int2x2 b({0,0},
			{0,0});
	EXPECT_FALSE(a<b);
}

TEST(mat2_tests,less_than_equal_true) {
	int2x2 a({0,0},
			{0,0});
	int2x2 b({1,2},
			{1,3});
	EXPECT_TRUE(a<=b);
}

TEST(mat2_tests,less_than_equal_false) {
	int2x2 a({1,2},
			{1,3});
	int2x2 b({0,0},
			{0,0});
	EXPECT_FALSE(a<=b);
}

TEST(mat2_tests,greater_than_true) {
	int2x2 a({1,2},
			{1,3});
	int2x2 b({0,0},
			{0,0});
	EXPECT_TRUE(a>b);
}

TEST(mat2_tests,greater_than_false) {
	int2x2 a({0,0},
			{0,0});
	int2x2 b({1,2},
			{1,3});
	EXPECT_FALSE(a>b);
}

TEST(mat2_tests,greater_than_equal_true) {
	int2x2 a({1,2},
			{1,3});
	int2x2 b({0,0},
			{0,0});
	EXPECT_TRUE(a>=b);
}

TEST(mat2_tests,greater_than_equal_false) {
	int2x2 a({0,0},
			{0,0});
	int2x2 b({1,2},
			{1,3});
	EXPECT_FALSE(a>=b);
}

TEST(mat2_tests,add) {
	int2x2 a({1,2},
			{1,3});
	int2x2 b({1,2},
			{1,3});
	EXPECT_EQ(int2x2({2,4},
					{2,6}),a+b);
}

TEST(mat2_tests,sub) {
	int2x2 a({2,4},
			{2,6});
	int2x2 b({1,2},
			{1,3});
	EXPECT_EQ(int2x2({1,2},
					{1,3}),a-b);
}

TEST(mat2_tests,neg) {
	int2x2 a({1,2},
			{1,3});
	EXPECT_EQ(int2x2({-1,-2},
					{-1,-3}),-a);
}

TEST(mat2_tests,mult_mat2) {
	int2x2 a({2,4},
			{2,6});
	int2x2 b({1,2},
			{1,3});
	EXPECT_EQ(int2x2({6, 16},
					 {8, 22}),a*b);
}

TEST(mat2_tests,mult_vec2) {
	int2x2 a({2,4},
			{2,6});
	int2 v(1,2);
	EXPECT_EQ(int2(10,14),a*v);
}

TEST(mat2_tests,mult_scaler) {
	int2x2 a({2,4},
			{2,6});
	int s = 2;
	EXPECT_EQ(int2x2({4, 8},
					{4, 12}),a*s);
}

TEST(mat2_tests,div_scaler) {
	int2x2 a({4, 8},
			{4, 12});
	int s = 2;
	EXPECT_EQ(int2x2({2,4},
					{2,6}),a/s);
}

TEST(mat2_tests,add_assign) {
	int2x2 a({1,2},
			{1,3});
	a += int2x2({1,2},
			   {1,3});
	EXPECT_EQ(int2x2({2,4},
					{2,6}),a);
}

TEST(mat2_tests,sub_assign) {
	int2x2 a({2,4},
			{2,6});
	a -= int2x2({1,2},
			   {1,3});
	EXPECT_EQ(int2x2({1,2},
					{1,3}),a);
}

TEST(mat2_tests,mult_assign_mat2) {
	int2x2 a({2,4},
			{2,6});
	a *= int2x2({1,2},
			   {1,3});
	EXPECT_EQ(int2x2({6, 16},
					{8, 22}),a);
}


TEST(mat2_tests,mult_assign_scaler) {
	int2x2 a({2,4},
			{2,6});
	a *= 2;
	EXPECT_EQ(int2x2({4, 8},
					{4, 12}),a);
}

TEST(mat2_tests,div_assign_scaler) {
	int2x2 a({4, 8},
			{4, 12});
	a /= 2;
	EXPECT_EQ(int2x2({2,4},
					{2,6}),a);
}

TEST(mat2_tests,det) {
	int2x2 a({2,4},
			{2,6});

	EXPECT_EQ(4,a.det());
}

TEST(mat2_tests,get_transpose) {
	int2x2 a({2,4},
			{2,6});

	EXPECT_EQ(int2x2({2,2},
					{4,6}), a.get_transpose());
}

TEST(mat2_tests,get_inverse) {
	int2x2 a({1,4},
			 {0,1});

	EXPECT_EQ(int2x2({1,-4},
					{0,1}), a.get_inverse());
}

TEST(mat2_tests,transpose) {
	int2x2 a({2,4},
			{2,6});

	a = a.transpose();

	EXPECT_EQ(int2x2({2,2},
					{4,6}),a);
}

TEST(mat2_tests,inverse) {
	int2x2 a({1,4},
			 {0,1});

	a.inverse();

	EXPECT_EQ(int2x2({1,-4},
					 {0, 1}),a);
}

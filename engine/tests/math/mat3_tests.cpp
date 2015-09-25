#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <cppbr/math/mat3.hpp>

TEST(mat3_tests, default_constructor) {
    int3x3 a;

    EXPECT_EQ(int3x3({1, 0, 0},
                     {0, 1, 0},
                     {0, 0, 1}), a);
}

TEST(mat3_tests, constructor_three_vec3s) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    EXPECT_EQ(int3x3({1, 2, 3},
                     {1, 3, 3},
                     {3, 2, 1}), a);
}

TEST(mat3_tests, equal_true) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    int3x3 b({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    EXPECT_TRUE(a == b);
}

TEST(mat3_tests, equal_false) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    int3x3 b({1, 2, 3},
             {4, 3, 3},
             {3, 2, 1});
    EXPECT_FALSE(a == b);
}

TEST(mat3_tests, not_equal_false) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    int3x3 b({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    EXPECT_FALSE(a != b);
}

TEST(mat3_tests, not_equal_true) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    int3x3 b({1, 2, 3},
             {1, 3, 3},
             {16, 2, 1});
    EXPECT_TRUE(a != b);
}

TEST(mat3_tests, less_than_true) {
    int3x3 a({0, 0, 0},
             {0, 0, 0},
             {0, 0, 0});
    int3x3 b({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    EXPECT_TRUE(a < b);
}

TEST(mat3_tests, less_than_false) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    int3x3 b({0, 0, 0},
             {0, 0, 0},
             {0, 0, 0});
    EXPECT_FALSE(a < b);
}

TEST(mat3_tests, less_than_equal_true) {
    int3x3 a({0, 0, 0},
             {0, 0, 0},
             {0, 0, 0});
    int3x3 b({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    EXPECT_TRUE(a <= b);
}

TEST(mat3_tests, less_than_equal_false) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    int3x3 b({0, 0, 0},
             {0, 0, 0},
             {0, 0, 0});
    EXPECT_FALSE(a <= b);
}

TEST(mat3_tests, greater_than_true) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    int3x3 b({0, 0, 0},
             {0, 0, 0},
             {0, 0, 0});
    EXPECT_TRUE(a > b);
}

TEST(mat3_tests, greater_than_false) {
    int3x3 a({0, 0, 0},
             {0, 0, 0},
             {0, 0, 0});
    int3x3 b({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    EXPECT_FALSE(a > b);
}

TEST(mat3_tests, greater_than_equal_true) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    int3x3 b({0, 0, 0},
             {0, 0, 0},
             {0, 0, 0});
    EXPECT_TRUE(a >= b);
}

TEST(mat3_tests, greater_than_equal_false) {
    int3x3 a({0, 0, 0},
             {0, 0, 0},
             {0, 0, 0});
    int3x3 b({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    EXPECT_FALSE(a >= b);
}

TEST(mat3_tests, add) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    int3x3 b({1, 2, 5},
             {1, 3, 3},
             {6, 12, 6});
    EXPECT_EQ(int3x3({2, 4, 8},
                     {2, 6, 6},
                     {9, 14, 7}), a + b);
}

TEST(mat3_tests, sub) {
    int3x3 a({2, 4, 8},
             {2, 6, 6},
             {9, 14, 7});
    int3x3 b({1, 2, 5},
             {1, 3, 3},
             {6, 12, 6});
    EXPECT_EQ(int3x3({1, 2, 3},
                     {1, 3, 3},
                     {3, 2, 1}), a - b);
}

TEST(mat3_tests, neg) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    EXPECT_EQ(int3x3({-1, -2, -3},
                     {-1, -3, -3},
                     {-3, -2, -1}), -a);
}

TEST(mat3_tests, mult_mat3) {
    int3x3 a({2, 4, 8},
             {2, 6, 6},
             {9, 14, 7});
    int3x3 b({1, 2, 5},
             {1, 3, 3},
             {6, 12, 6});
    EXPECT_EQ(int3x3({{54, 112, 70},
                      {44, 94,  64},
                      {65, 144, 129}}), a * b);
}

TEST(mat3_tests, mult_vec3) {
    int3x3 a({2, 4, 8},
             {2, 6, 6},
             {9, 14, 7});
    int3 v(1, 2, 3);
    EXPECT_EQ(int3(34, 32, 58), a * v);
}

TEST(mat3_tests, mult_scaler) {
    int3x3 a({2, 4, 8},
             {2, 6, 6},
             {9, 14, 7});
    int s = 2;
    EXPECT_EQ(int3x3({4, 8, 16},
                     {4, 12, 12},
                     {18, 28, 14}), a * s);
}

TEST(mat3_tests, div_scaler) {
    int3x3 a({4, 8, 16},
             {4, 12, 12},
             {18, 28, 14});
    int s = 2;
    EXPECT_EQ(int3x3({2, 4, 8},
                     {2, 6, 6},
                     {9, 14, 7}), a / s);
}

TEST(mat3_tests, add_assign) {
    int3x3 a({1, 2, 3},
             {1, 3, 3},
             {3, 2, 1});
    a += int3x3({1, 2, 5},
                {1, 3, 3},
                {6, 12, 6});
    EXPECT_EQ(int3x3({2, 4, 8},
                     {2, 6, 6},
                     {9, 14, 7}), a);
}

TEST(mat3_tests, sub_assign) {
    int3x3 a({2, 4, 8},
             {2, 6, 6},
             {9, 14, 7});
    a -= int3x3({1, 2, 5},
                {1, 3, 3},
                {6, 12, 6});
    EXPECT_EQ(int3x3({1, 2, 3},
                     {1, 3, 3},
                     {3, 2, 1}), a);
}

TEST(mat3_tests, mult_assign_mat3) {
    int3x3 a({2, 4, 8},
             {2, 6, 6},
             {9, 14, 7});
    a *= int3x3({1, 2, 5},
                {1, 3, 3},
                {6, 12, 6});
    EXPECT_EQ(int3x3({{54, 112, 70},
                      {44, 94,  64},
                      {65, 144, 129}}), a);
}


TEST(mat3_tests, mult_assign_scaler) {
    int3x3 a({2, 4, 8},
             {2, 6, 6},
             {9, 14, 7});
    a *= 2;
    EXPECT_EQ(int3x3({4, 8, 16},
                     {4, 12, 12},
                     {18, 28, 14}), a);
}

TEST(mat3_tests, div_assign_scaler) {
    int3x3 a({4, 8, 16},
             {4, 12, 12},
             {18, 28, 14});
    a /= 2;
    EXPECT_EQ(int3x3({2, 4, 8},
                     {2, 6, 6},
                     {9, 14, 7}), a);
}

TEST(mat3_tests, det) {
    int3x3 a({2, 4, 8},
             {2, 6, 6},
             {9, 14, 7});

    EXPECT_EQ(-132, a.det());
}

TEST(mat3_tests, get_transpose) {
    int3x3 a({2, 4, 8},
             {2, 6, 6},
             {9, 14, 7});

    EXPECT_EQ(int3x3({2, 2, 9},
                     {4, 6, 14},
                     {8, 6, 7}), a.get_transpose());
}

TEST(mat3_tests, get_inverse) {
    int3x3 a({1, 4, 8},
             {0, 1, 6},
             {0, 0, 1});

    EXPECT_EQ(int3x3({1, -4, 16},
                     {0, 1, -6},
                     {0, 0, 1}), a.get_inverse());
}

TEST(mat3_tests, transpose) {
    int3x3 a({2, 4, 8},
             {2, 6, 6},
             {9, 14, 7});

    a = a.transpose();

    EXPECT_EQ(int3x3({2, 2, 9},
                     {4, 6, 14},
                     {8, 6, 7}), a);
}

TEST(mat3_tests, inverse) {
    int3x3 a({1, 4, 8},
             {0, 1, 6},
             {0, 0, 1});

    a.inverse();

    EXPECT_EQ(int3x3({1, -4, 16},
                     {0, 1, -6},
                     {0, 0, 1}), a);
}

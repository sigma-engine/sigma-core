#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/vec3.hpp>
#include <sigmafive/math/vec4.hpp>

TEST(vec4_tests, default_constructor) {
    int4 a;
    EXPECT_EQ(int4(0, 0, 0, 0), a);
}

TEST(vec4_tests, constructor_four_values) {
    int4 a(1, 2, 3, 4);
    EXPECT_EQ(int4(1, 2, 3, 4), a);
}

TEST(vec4_tests, constructor_vec2_and_two_values) {
    int4 a(int2(1, 2), 3, 4);
    EXPECT_EQ(int4(1, 2, 3, 4), a);
}

TEST(vec4_tests, constructor_two_vec2s) {
    int4 a(int2(1, 2), int2(3, 4));
    EXPECT_EQ(int4(1, 2, 3, 4), a);
}

TEST(vec4_tests, constructor_vec3_and_a_value) {
    int4 a(int3(1, 2, 3), 4);
    EXPECT_EQ(int4(1, 2, 3, 4), a);
}

TEST(vec4_tests, equal_true) {
    int4 a(1, 5, 1, 2);
    int4 b(1, 5, 1, 2);
    EXPECT_TRUE(a == b);
}

TEST(vec4_tests, equal_false) {
    int4 a(1, 5, 1, 2);
    int4 b(1, 5, 1, 3);
    EXPECT_FALSE(a == b);
}

TEST(vec4_tests, not_equal_false) {
    int4 a(1, 5, 1, 2);
    int4 b(1, 5, 1, 2);
    EXPECT_FALSE(a != b);
}

TEST(vec4_tests, not_equal_true) {
    int4 a(1, 5, 1, 2);
    int4 b(1, 5, 1, 3);
    EXPECT_TRUE(a != b);
}

TEST(vec4_tests, less_than_true) {
    int4 a(1, 5, 1, 2);
    int4 b(2, 7, 43, 55);
    EXPECT_TRUE(a < b);
}

TEST(vec4_tests, less_than_false) {
    int4 a(2, 7, 43, 55);
    int4 b(1, 5, 1, 2);
    EXPECT_FALSE(a < b);
}

TEST(vec4_tests, less_than_equal_true) {
    int4 a(1, 5, 1, 2);
    int4 b(2, 7, 43, 55);
    EXPECT_TRUE(a <= b);
}

TEST(vec4_tests, less_than_equal_false) {
    int4 a(2, 7, 43, 55);
    int4 b(1, 5, 1, 2);
    EXPECT_FALSE(a <= b);
}

TEST(vec4_tests, greater_than_true) {
    int4 a(2, 7, 43, 55);
    int4 b(1, 5, 1, 2);
    EXPECT_TRUE(a > b);
}

TEST(vec4_tests, greater_than_false) {
    int4 a(1, 5, 1, 2);
    int4 b(2, 7, 43, 55);
    EXPECT_FALSE(a > b);
}

TEST(vec4_tests, greater_than_equal_true) {
    int4 a(2, 7, 43, 55);
    int4 b(1, 5, 1, 2);
    EXPECT_TRUE(a >= b);
}

TEST(vec4_tests, greater_than_equal_false) {
    int4 a(1, 5, 1, 2);
    int4 b(2, 7, 43, 55);
    EXPECT_FALSE(a >= b);
}

TEST(vec4_tests, add_a_add_b) {
    int4 a(1, 5, 1, 2);
    int4 b(2, 7, 43, 55);
    EXPECT_EQ(int4(3, 12, 44, 57), a + b);
}

TEST(vec4_tests, sub_a_sub_b) {
    int4 a(1, 5, 1, 2);
    int4 b(2, 7, 43, 55);
    EXPECT_EQ(int4(-1, -2, -42, -53), a - b);
}

TEST(vec4_tests, neg_a) {
    int4 a(1, 5, 1, 2);
    EXPECT_EQ(int4(-1, -5, -1, -2), -a);
}

TEST(vec4_tests, mult_a_times_scaler) {
    int4 a(1, 5, 1, 2);
    int s = 15;
    EXPECT_EQ(int4(15, 75, 15, 30), a * s);
}

TEST(vec4_tests, div_a_div_scaler) {
    int4 a(15, 75, 15, 30);
    int s = 15;
    EXPECT_EQ(int4(1, 5, 1, 2), a / s);
}

TEST(vec4_tests, add_assign_a_add_b) {
    int4 a(1, 5, 1, 2);
    a += int4(2, 7, 43, 55);
    EXPECT_EQ(int4(3, 12, 44, 57), a);
}

TEST(vec4_tests, sub_assign_a_sub_b) {
    int4 a(1, 5, 1, 2);
    a -= int4(2, 7, 43, 55);
    EXPECT_EQ(int4(-1, -2, -42, -53), a);
}

TEST(vec4_tests, mult_assign_a_times_scaler) {
    int4 a(1, 5, 1, 2);
    a *= 15;
    EXPECT_EQ(int4(15, 75, 15, 30), a);
}

TEST(vec4_tests, div_assign_scaler) {
    int4 a(15, 75, 15, 30);
    a /= 15;
    EXPECT_EQ(int4(1, 5, 1, 2), a);
}

TEST(vec4_tests, length) {
    float4 a(1, 5, 1, 2);
    EXPECT_FLOAT_EQ(std::sqrt(31.0f), a.length());
}

TEST(vec4_tests, square_length) {
    float4 a(1, 5, 1, 2);
    EXPECT_FLOAT_EQ(31.0f, a.square_length());
}

TEST(vec4_tests, get_normalized) {
    float4 a(1, 5, 1, 2);
    auto result = a.get_normalized();
    EXPECT_EQ(a / std::sqrt(31.0f), result);
}

TEST(vec4_tests, dot) {
    float4 a(1, 5, 1, 2);
    float4 b(1, 7, 3, 2);

    EXPECT_FLOAT_EQ(43.0f, a.dot(b));
}

TEST(vec4_tests, normalize) {
    float4 a(1, 5, 1, 2);
    float4 expected = a / std::sqrt(31.0f);

    a.normalize();
    EXPECT_EQ(expected, a);
}

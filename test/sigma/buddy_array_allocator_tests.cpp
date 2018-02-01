#include <sigma/buddy_array_allocator.hpp>

#include <gtest/gtest.h>

TEST(buddy_array_allocator, order_with_1_block_should_return_0)
{
    sigma::buddy_array_allocator allocator(16);

    EXPECT_EQ(0, allocator.order(1));
}

TEST(buddy_array_allocator, order_with_2_block_should_return_1)
{
    sigma::buddy_array_allocator allocator(16);

    EXPECT_EQ(1, allocator.order(2));
}

TEST(buddy_array_allocator, order_with_3_block_should_return_2)
{
    sigma::buddy_array_allocator allocator(16);

    EXPECT_EQ(2, allocator.order(3));
}

TEST(buddy_array_allocator, order_with_15_block_should_return_4)
{
    sigma::buddy_array_allocator allocator(16);

    EXPECT_EQ(4, allocator.order(15));
}

TEST(buddy_array_allocator, allocate_17_blocks_should_return_neg_1)
{
    sigma::buddy_array_allocator allocator(16);

    EXPECT_EQ(-1, allocator.allocate(17));
}

TEST(buddy_array_allocator, allocate_16_blocks_should_return_0)
{
    sigma::buddy_array_allocator allocator(16);

    EXPECT_EQ(0, allocator.allocate(16));
}

TEST(buddy_array_allocator, allocate_8_blocks_should_return_0)
{
    sigma::buddy_array_allocator allocator(16);

    EXPECT_EQ(0, allocator.allocate(8));
}

TEST(buddy_array_allocator, allocate_3_8_blocks_should_return_0_8_neg_1)
{
    sigma::buddy_array_allocator allocator(16);

    for (int i = 0; i < 2; ++i)
        EXPECT_EQ(8 * i, allocator.allocate(8));

    EXPECT_EQ(-1, allocator.allocate(8));
}

TEST(buddy_array_allocator, allocate_5_4_blocks_should_return_0_4_8_12_neg_1)
{
    sigma::buddy_array_allocator allocator(16);

    for (int i = 0; i < 4; ++i)
        EXPECT_EQ(4 * i, allocator.allocate(4));

    EXPECT_EQ(-1, allocator.allocate(4));
}

TEST(buddy_array_allocator, allocate_9_2_blocks_should_return_even_numbers_up_to_14__neg_1)
{
    sigma::buddy_array_allocator allocator(16);

    for (int i = 0; i < 8; ++i)
        EXPECT_EQ(2 * i, allocator.allocate(2));

    EXPECT_EQ(-1, allocator.allocate(2));
}

TEST(buddy_array_allocator, allocate_17_1_blocks_should_return_all_numbers_and_neg_1)
{
    sigma::buddy_array_allocator allocator(16);

    for (int i = 0; i < 16; ++i)
        EXPECT_EQ(i, allocator.allocate(1));

    EXPECT_EQ(-1, allocator.allocate(1));
}

TEST(buddy_array_allocator, allocate_2_4_blocks_1_8_block_and_1_4_block_should_return_0_4_8_neg_1)
{
    sigma::buddy_array_allocator allocator(16);

    for (int i = 0; i < 2; ++i)
        EXPECT_EQ(4 * i, allocator.allocate(4));

    EXPECT_EQ(8, allocator.allocate(8));
    EXPECT_EQ(-1, allocator.allocate(4));
}

TEST(buddy_array_allocator, allocate_1_8_block_2_4_blocks_and_1_4_block_should_return_0_8_12_neg_1)
{
    sigma::buddy_array_allocator allocator(16);

    EXPECT_EQ(0, allocator.allocate(8));
    EXPECT_EQ(8, allocator.allocate(4));
    EXPECT_EQ(12, allocator.allocate(4));
    EXPECT_EQ(-1, allocator.allocate(4));
}

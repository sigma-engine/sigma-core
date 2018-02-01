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

TEST(buddy_array_allocator, deallocate_before_allocate_return_false)
{
    sigma::buddy_array_allocator allocator(16);
    EXPECT_FALSE(allocator.deallocate(0));
}

TEST(buddy_array_allocator, deallocate_after_allocate_return_true)
{
    sigma::buddy_array_allocator allocator(16);
    std::size_t x1 = allocator.allocate(8);
    EXPECT_TRUE(allocator.deallocate(x1));
}

TEST(buddy_array_allocator, deallocate_reuses_block_after_free)
{
    sigma::buddy_array_allocator allocator(16);
    std::size_t x1 = allocator.allocate(8);
    std::size_t x2 = allocator.allocate(8);

    EXPECT_TRUE(allocator.deallocate(x2));
    EXPECT_EQ(x2, allocator.allocate(8));
}

TEST(buddy_array_allocator, deallocate_after_16_1_blocks_true)
{
    sigma::buddy_array_allocator allocator(16);
    for (std::size_t i = 0; i < 16; i++)
        allocator.allocate(1);

    for (std::size_t i = 0; i < 16; i++)
        EXPECT_TRUE(allocator.deallocate(i));
}

TEST(buddy_array_allocator, deallocate_after_8_2_blocks_true)
{
    sigma::buddy_array_allocator allocator(16);
    for (std::size_t i = 0; i < 8; i++)
        allocator.allocate(2);

    for (std::size_t i = 0; i < 8; i++)
        EXPECT_TRUE(allocator.deallocate(2 * i));
}

TEST(buddy_array_allocator, deallocate_after_4_4_blocks_true)
{
    sigma::buddy_array_allocator allocator(16);
    for (std::size_t i = 0; i < 4; i++)
        allocator.allocate(4);

    for (std::size_t i = 0; i < 4; i++)
        EXPECT_TRUE(allocator.deallocate(4 * i));
}

TEST(buddy_array_allocator, deallocate_after_2_8_blocks_true)
{
    sigma::buddy_array_allocator allocator(16);
    for (std::size_t i = 0; i < 2; i++)
        allocator.allocate(8);

    for (std::size_t i = 0; i < 2; i++)
        EXPECT_TRUE(allocator.deallocate(8 * i));
}

TEST(buddy_array_allocator, deallocate_reuses_one_level_of_merged_blocks_after_free)
{
    sigma::buddy_array_allocator allocator(16);
    std::size_t x1 = allocator.allocate(4);
    std::size_t x2 = allocator.allocate(4);
    std::size_t x3 = allocator.allocate(4);
    std::size_t x4 = allocator.allocate(4);

    EXPECT_TRUE(allocator.deallocate(x3));
    EXPECT_TRUE(allocator.deallocate(x4));
    EXPECT_EQ(x3, allocator.allocate(8));
}

TEST(buddy_array_allocator, deallocate_reuses_two_level_of_merged_blocks_after_free)
{
    sigma::buddy_array_allocator allocator(16);
    std::size_t x1 = allocator.allocate(2);
    std::size_t x2 = allocator.allocate(2);
    std::size_t x3 = allocator.allocate(2);
    std::size_t x4 = allocator.allocate(2);

    std::size_t x5 = allocator.allocate(2);
    std::size_t x6 = allocator.allocate(2);
    std::size_t x7 = allocator.allocate(2);
    std::size_t x8 = allocator.allocate(2);

    EXPECT_TRUE(allocator.deallocate(x5));
    EXPECT_TRUE(allocator.deallocate(x6));
    EXPECT_TRUE(allocator.deallocate(x7));
    EXPECT_TRUE(allocator.deallocate(x8));
    EXPECT_EQ(x5, allocator.allocate(8));
}

TEST(buddy_array_allocator, deallocate_reuses_three_level_of_merged_blocks_after_free)
{
    sigma::buddy_array_allocator allocator(16);
    std::size_t x1 = allocator.allocate(2);
    std::size_t x2 = allocator.allocate(2);
    std::size_t x3 = allocator.allocate(2);
    std::size_t x4 = allocator.allocate(2);

    std::size_t x5 = allocator.allocate(2);
    std::size_t x6 = allocator.allocate(2);
    std::size_t x7 = allocator.allocate(2);
    std::size_t x8 = allocator.allocate(2);

    EXPECT_TRUE(allocator.deallocate(x1));
    EXPECT_TRUE(allocator.deallocate(x2));
    EXPECT_TRUE(allocator.deallocate(x3));
    EXPECT_TRUE(allocator.deallocate(x4));

    EXPECT_TRUE(allocator.deallocate(x5));
    EXPECT_TRUE(allocator.deallocate(x6));
    EXPECT_TRUE(allocator.deallocate(x7));
    EXPECT_TRUE(allocator.deallocate(x8));
    EXPECT_EQ(x1, allocator.allocate(16));
}

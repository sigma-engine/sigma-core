#include <sigma/buddy_array_allocator.hpp>

#define IS_USED(X) (X & 1)
#define SET_USED(X) (X |= 1)

#define IS_SPLIT(X) (X & 2)
#define SET_SPLIT(X) (X |= 2)

#include <cmath>

namespace sigma {

buddy_array_allocator::buddy_array_allocator(std::size_t block_count)
    : block_count_(block_count)
    , nodes_(2 * block_count - 1, 0)
{
}

std::size_t buddy_array_allocator::order(std::size_t blocks) const noexcept
{
    return std::ceil(std::log2(blocks));
}

std::size_t buddy_array_allocator::allocate(std::size_t blocks)
{
    return allocate_(blocks, 0, 0, block_count_);
}

std::size_t buddy_array_allocator::allocate_(std::size_t blocks, std::size_t node, std::size_t left, std::size_t size)
{
    if (IS_USED(nodes_[node]))
        return -1;

    std::size_t lower = size / 2;
    if (blocks > size) {
        return -1;
    } else if (lower < blocks) {
        if (IS_SPLIT(nodes_[node]))
            return -1;

        SET_USED(nodes_[node]);
        return left;
    } else {
        SET_SPLIT(nodes_[node]);

        std::size_t l = allocate_(blocks, 2 * node + 1, left, lower);
        if (l != -1)
            return l;
        else {
            return allocate_(blocks, 2 * node + 2, left + lower, lower);
        }
    }

    return -1;
}
}

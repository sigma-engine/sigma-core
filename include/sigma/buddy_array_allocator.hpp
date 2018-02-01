#ifndef SIGMA_BUDDY_ARRAY_ALLOCATOR_HPP
#define SIGMA_BUDDY_ARRAY_ALLOCATOR_HPP

#include <cstdint>
#include <vector>

namespace sigma {
class buddy_array_allocator {
    std::size_t block_count_;

    std::vector<std::uint8_t> nodes_;

public:
    buddy_array_allocator(std::size_t block_count);

    buddy_array_allocator(buddy_array_allocator&&) = delete;

    ~buddy_array_allocator() = default;

    buddy_array_allocator& operator=(buddy_array_allocator&&) = delete;

    std::size_t order(std::size_t blocks) const noexcept;

    std::size_t allocate(std::size_t blocks);

private:
    buddy_array_allocator(const buddy_array_allocator&) = delete;
    buddy_array_allocator& operator=(const buddy_array_allocator&) = delete;

    std::size_t allocate_(std::size_t blocks, std::size_t node, std::size_t left, std::size_t size);
};
}

#endif // SIGMA_BUDDY_ARRAY_ALLOCATOR_HPP

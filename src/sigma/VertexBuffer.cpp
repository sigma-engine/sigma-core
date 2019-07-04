#include <sigma/VertexBuffer.hpp>

VertexLayout::VertexLayout(const std::vector<VertexMember>& inMembers, std::size_t inStride)
    : mStride(inStride)
    , mMembers(inMembers)
{
}

std::size_t VertexLayout::stride() const
{
    return mStride;
}

std::vector<VertexMember>::const_iterator VertexLayout::begin() const { return mMembers.begin(); }

std::vector<VertexMember>::const_iterator VertexLayout::end() const { return mMembers.end(); }

#include <sigma/VertexLayout.hpp>

#include <sigma/DataTypes.hpp>

VertexLayout::VertexLayout(const std::initializer_list<VertexMemberDescription>& inLayout)
{
    mStride = 0;
    for (const auto& des : inLayout) {
        mMembers.push_back({ mStride,
            sizeOfDataType(des.type),
            des.type,
            des.name,
            des.normalized });
        mStride += mMembers.back().size;
    }
}

VertexLayout::VertexLayout(const std::vector<VertexMember>& inMembers, uint32_t inStride)
    : mStride(inStride)
    , mMembers(inMembers)
{
}

uint32_t VertexLayout::stride() const
{
    return mStride;
}

std::vector<VertexMember>::const_iterator VertexLayout::begin() const { return mMembers.begin(); }

std::vector<VertexMember>::const_iterator VertexLayout::end() const { return mMembers.end(); }
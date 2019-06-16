#pragma once

#include <sigma/DataTypes.hpp>

#include <cstdint>
#include <string>
#include <vector>

struct VertexMemberDescription
{
    DataType type;
    std::string name;
    bool normalized = false;
};

struct VertexMember
{
    std::size_t offset;
    std::size_t size;
    DataType type;
    std::string name;
    bool normalized;
};

class VertexLayout
{
public:
    VertexLayout(const std::vector<VertexMember> &inMembers, std::size_t inStride);

    std::size_t stride() const;

    std::vector<VertexMember>::const_iterator begin() const;

    std::vector<VertexMember>::const_iterator end() const;
private:
    std::size_t mStride;
    std::vector<VertexMember> mMembers;
};

class VertexBuffer
{
public:
	virtual ~VertexBuffer() = default;

    virtual const VertexLayout &layout() const = 0;

    virtual void setData(const void *inData, std::size_t inSize) = 0;
};

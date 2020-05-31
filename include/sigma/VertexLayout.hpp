#pragma once

#include <sigma/DataTypes.hpp>

#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

struct VertexMemberDescription {
	uint32_t location;
	DataType type;
	std::string name;
	bool normalized = false;
};

struct VertexMember {
	uint32_t location;
	uint32_t offset;
	uint32_t size;
	DataType type;
	std::string name;
	bool normalized;
};

class VertexLayout {
public:
	VertexLayout(const std::initializer_list<VertexMemberDescription>& inLayout);

	VertexLayout(const std::vector<VertexMember>& inMembers, uint32_t inStride);

	uint32_t stride() const;

	uint32_t attributeCount() const { return static_cast<uint32_t>(mMembers.size()); };

	std::vector<VertexMember>::const_iterator begin() const;

	std::vector<VertexMember>::const_iterator end() const;

private:
	uint32_t mStride;
	std::vector<VertexMember> mMembers;
};

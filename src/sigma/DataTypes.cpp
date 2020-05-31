#include <sigma/DataTypes.hpp>

#include <sigma/Log.hpp>

uint32_t sizeOfDataType(DataType inType)
{
	switch (inType) {
	case DataType::Float:
		return 1 * sizeof(float);
	case DataType::Vec2:
		return 2 * sizeof(float);
	case DataType::Vec3:
		return 3 * sizeof(float);
	case DataType::Vec4:
		return 4 * sizeof(float);
	case DataType::UInt:
		return 1 * sizeof(uint32_t);
	case DataType::UShort:
		return 1 * sizeof(uint16_t);
	}
	SIGMA_ASSERT(false, "Unknown DataType");
	return 0;
}

uint32_t componentCountOfDataType(DataType inType)
{
	switch (inType) {
	case DataType::Float:
		return 1;
	case DataType::Vec2:
		return 2;
	case DataType::Vec3:
		return 3;
	case DataType::Vec4:
		return 4;
	case DataType::UInt:
		return 1;
	case DataType::UShort:
		return 1;
	}
	SIGMA_ASSERT(false, "Unknown DataType");
	return 0;
}
#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

class UniformBuffer;

enum class DescriptorType {
    UniformBuffer
};

struct DescriptorSetLayoutBinding {
    uint32_t binding;
    DescriptorType type;
    uint32_t count;
};

class DescriptorSetLayout {
public:
    virtual ~DescriptorSetLayout() = default;
};

struct DescriptorSetCreateParams {
    std::shared_ptr<DescriptorSetLayout> layout;
    std::unordered_map<uint32_t, std::shared_ptr<UniformBuffer>> uniformBuffers;
};

class DescriptorSet {
public:
    virtual ~DescriptorSet() = default;

    virtual std::shared_ptr<DescriptorSetLayout> layout() const = 0;
};
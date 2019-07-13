#pragma once

#include <vector>

#include <sigma/Texture.hpp>

enum class AttachmentType {
    ColorAttachment
};

struct RenderPassAttachment {
    AttachmentType type;
    ImageFormat format;
};

struct RenderPassCreateParams {
    std::vector<RenderPassAttachment> attachments;
};

class RenderPass {
public:
    virtual ~RenderPass() = default;

	virtual const std::vector<RenderPassAttachment> &attachments() const = 0;
private:
};

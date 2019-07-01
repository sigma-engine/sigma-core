#pragma once

#include <vector>

enum class ImageFormat
{
    Unknown,
    UnormB8G8R8A8
};

enum class AttachmentType
{
    ColorAttachment
};

struct RenderPassAttachment
{
    AttachmentType type;
    ImageFormat format;
};

struct RenderPassCreateParams
{
    std::vector<RenderPassAttachment> attachments;
};

class RenderPass {
public:
    virtual ~RenderPass() = default;
};

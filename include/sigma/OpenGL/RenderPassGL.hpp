#pragma once

#include <sigma/RenderPass.hpp>

class RenderPassGL : public RenderPass {
public:
    const std::vector<RenderPassAttachment>& attachments() const override;

    bool initialize(const RenderPassCreateParams& inParams);

private:
    std::vector<RenderPassAttachment> mAttachments;
};
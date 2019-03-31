#pragma once

#include <sigma/config.hpp>

#include <glm/mat4x4.hpp>

static const constexpr uint64_t MAX_RENDER_TARGETS = 4;
static const constexpr uint64_t MAX_TEXTURE_BINDINGS = 16;
static const constexpr uint64_t MAX_BUFFER_BINDINGS = 16;

namespace sigma {
namespace graphics {
    struct render_command {
        uint64_t group;

        uint64_t program;
        uint64_t buffer_bindings[MAX_BUFFER_BINDINGS];
        uint64_t input_textures[MAX_TEXTURE_BINDINGS];

        uint64_t mesh;
        uint64_t offset;
        uint64_t count;

        glm::mat4 model;
        glm::mat4 projection_view;
    };

    class render_queue {
    public:
        render_queue(uint64_t id)
            : id_(id)
        {
        }

        virtual ~render_queue() {}

        uint64_t id() const { return id_; }

        virtual render_command* enqueue() { return nullptr; }

    private:
        uint64_t id_;
    };
}
}
#ifndef SIGMAFIVE_EDITOR_WIDGETS_GAME_VIEW_RENDERER_HPP
#define SIGMAFIVE_EDITOR_WIDGETS_GAME_VIEW_RENDERER_HPP

#include <editor/config.hpp>

#include <sigmafive/graphics/context.hpp>
#include <sigmafive/graphics/context_manager.hpp>

#include <QtQuick/QQuickFramebufferObject>

namespace sigmafive {
    namespace editor {
        namespace widgets {
            class GameView;

            class EDITOR_API GameViewRenderer : public QQuickFramebufferObject::Renderer {
            public:
                static constexpr const class_uid CONTEXT_UID = sigmafive::compile_time_hash(
                "sigmafive::graphics::opengl::context");

                GameViewRenderer(graphics::context_manager *context_manager);

                QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);

                void synchronize(QQuickFramebufferObject *item);

                void render();

            private:
                GameView *item_;

                std::unique_ptr<sigmafive::graphics::context> context_;

                float4x4 view_matrix_;
                float4x4 projection_matrix_;
            };
        }
    }
}


#endif //SIGMAFIVE_EDITOR_WIDGETS_GAME_VIEW_RENDERER_HPP

#ifndef SIGMA_GAME_VIEW_HPP
#define SIGMA_GAME_VIEW_HPP

#include <QtQuick/QQuickFramebufferObject>

#include <EditorContext.hpp>

namespace sigma {
class GameView : public QQuickFramebufferObject {
    Q_OBJECT
public:
    Q_PROPERTY(EditorContext* activeContext READ activeContext WRITE setActiveContext NOTIFY activeContextChanged)

    GameView(QQuickItem* parent = 0);

    ~GameView();

    EditorContext* activeContext();

    void setActiveContext(EditorContext* ctx);

    QQuickFramebufferObject::Renderer* createRenderer() const override;
signals:
    void activeContextChanged(EditorContext* ctx);

private:
    EditorContext* activeContext_;
};
}

#endif //SIGMA_GAME_VIEW_HPP

#ifndef SIGMA_GAME_VIEW_HPP
#define SIGMA_GAME_VIEW_HPP

#include <QtQuick/QQuickFramebufferObject>

#include <EditorContext.hpp>
#include <TrackballController.hpp>

namespace sigma {
class GameView : public QQuickFramebufferObject {
    Q_OBJECT
public:
    Q_PROPERTY(EditorContext* activeContext READ activeContext WRITE setActiveContext NOTIFY activeContextChanged)
    Q_PROPERTY(TrackballController* controller READ controller WRITE setController NOTIFY controllerChanged)

    GameView(QQuickItem* parent = 0);

    ~GameView();

    QQuickFramebufferObject::Renderer* createRenderer() const override;

    EditorContext* activeContext();

    void setActiveContext(EditorContext* ctx);

    TrackballController* controller() const;

    void setController(TrackballController* ctl);

signals:
    void activeContextChanged(EditorContext* ctx);

    void controllerChanged(TrackballController* ctr);

private:
    EditorContext* activeContext_;
    TrackballController* controller_;
};
}

#endif //SIGMA_GAME_VIEW_HPP

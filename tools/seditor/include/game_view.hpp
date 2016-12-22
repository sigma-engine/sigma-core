#ifndef SIGMA_GAME_VIEW_HPP
#define SIGMA_GAME_VIEW_HPP

#include <QtQuick/QQuickFramebufferObject>

#include <qt_context.hpp>

namespace sigma {
class game_view : public QQuickFramebufferObject {
    Q_OBJECT
public:
    Q_PROPERTY(qt_context* activeContext READ activeContext WRITE setActiveContext NOTIFY activeContextChanged)

    game_view(QQuickItem* parent = 0);

	~game_view();

	void releaseResources() Q_DECL_OVERRIDE;

    qt_context* activeContext();

    void setActiveContext(qt_context* ctx);

    QQuickFramebufferObject::Renderer* createRenderer() const override;

    QSGNode* updatePaintNode(QSGNode* node, QQuickItem::UpdatePaintNodeData* nodeData) override;
signals:
    void activeContextChanged(qt_context* ctx);

private:
    qt_context* activeContext_;
};
}

#endif //SIGMA_GAME_VIEW_HPP

#ifndef SIGMA_QT_CONTEXT_HPP
#define SIGMA_QT_CONTEXT_HPP

#include <QObject>
#include <sigma/context.hpp>

namespace sigma {
class qt_context : public QObject, public sigma::context {
    Q_OBJECT
public:
    Q_INVOKABLE explicit qt_context(QObject* parent = nullptr);

    virtual void render(glm::ivec2 size) override;

    void release_renderer();
};
}

#endif // SIGMA_QT_CONTEXT_HPP

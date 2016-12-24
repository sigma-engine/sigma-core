#ifndef SIGMA_EDITOR_CONTEXT_HPP
#define SIGMA_EDITOR_CONTEXT_HPP

#include <QObject>
#include <sigma/context.hpp>

namespace sigma {
class EditorContext : public QObject, public sigma::context {
    Q_OBJECT
public:
    explicit EditorContext(QObject* parent = nullptr);
};
}

#endif // SIGMA_EDITOR_CONTEXT_HPP

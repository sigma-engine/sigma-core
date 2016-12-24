#ifndef SIGMA_QT_CONTEXT_HPP
#define SIGMA_QT_CONTEXT_HPP

#include <QObject>
#include <sigma/context.hpp>

namespace sigma {
class qt_context : public QObject, public sigma::context {
    Q_OBJECT
public:
    explicit qt_context(QObject* parent = nullptr);
};
}

#endif // SIGMA_QT_CONTEXT_HPP

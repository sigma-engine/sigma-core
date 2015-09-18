#ifndef SIGMAFIVE_EDITOR_ENTITY_MANAGER_MODEL_HPP
#define SIGMAFIVE_EDITOR_ENTITY_MANAGER_MODEL_HPP

#include <editor/config.hpp>

#include <QAbstractListModel>
#include <sigmafive/game/entity_manager.hpp>

namespace sigmafive {
    namespace editor {
        //https://developer.ubuntu.com/api/apps/qml/sdk-14.10/QtQuick.qtquick-modelviewsdata-cppmodels/
        class EDITOR_API entity_manager : public QAbstractListModel, public game::entity_manager {

        Q_OBJECT
        public:
            enum AnimalRoles {
                ID_ROLE = Qt::UserRole + 1
            };

            explicit entity_manager(QObject *parent = nullptr);

            virtual game::entity create() override;

            virtual void destroy(game::entity e) override;

            int rowCount(const QModelIndex &parent = QModelIndex()) const;

            QVariant data(const QModelIndex &index, int role) const;

            QHash<int, QByteArray> roleNames() const;
        private:
        };
    }
}

#endif //SIGMAFIVE_EDITOR_ENTITY_MANAGER_MODEL_HPP

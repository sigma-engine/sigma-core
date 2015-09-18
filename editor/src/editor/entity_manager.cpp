#include <editor/component_manager.hpp>
#include <editor/entity_manager.hpp>

namespace sigmafive {
    namespace editor {
        entity_manager::entity_manager(QObject *parent)
                : QAbstractListModel(parent) {
        }

        int entity_manager::rowCount(const QModelIndex &parent) const {
            return size();
        }

        QVariant entity_manager::data(const QModelIndex &index, int role) const {
            if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
                return QVariant();
            switch (role) {
                case ID_ROLE: {
                    return QVariant(QString::number(index.row() + 1));
                    break;
                }
                case Qt::DisplayRole: {
                    return QVariant("Entity: " + QString::number(index.row() + 1));
                    break;
                }
                default: {
                    return QVariant();
                    break;
                }
            }
        }

        QHash<int, QByteArray> entity_manager::roleNames() const {
            QHash<int, QByteArray> roles;
            roles[ID_ROLE] = "ID";
            return roles;
        }

        game::entity entity_manager::create() {
            beginInsertRows(QModelIndex(),size(),size()+1);
            auto e = game::entity_manager::create();
            endInsertRows();
            return e;
        }

        void entity_manager::destroy(game::entity e) {
            beginRemoveRows(QModelIndex(),size(),size());
            game::entity_manager::destroy(e);
            endRemoveRows();
        }
    }
}

#include <editor/entity_manager_model.hpp>

namespace sigmafive {
    namespace editor {

        entity_manager_model::entity_manager_model(game::entity_manager &entity_manager,
                                                   game::component_manager &component_manager,
                                                   game::component_system_manager &component_system_manager,
                                                   system::resource_manager &resource_manager,
                                                   graphics::context_manager &context_manager,
                                                   QObject *parent)
                : QAbstractListModel(parent),
                  entity_manager_(entity_manager),
                  component_manager_(component_manager),
                  component_system_manager_(component_system_manager),
                  resource_manager_(resource_manager),
                  context_manager_(context_manager) {
        }

        int entity_manager_model::rowCount(const QModelIndex &parent) const {
            return entity_manager_.size();
        }

        QVariant entity_manager_model::data(const QModelIndex &index, int role) const {
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
    }
}

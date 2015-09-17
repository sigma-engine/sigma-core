#ifndef SIGMAFIVE_EDITOR_ENTITY_MANAGER_MODEL_HPP
#define SIGMAFIVE_EDITOR_ENTITY_MANAGER_MODEL_HPP

#include <editor/config.hpp>

#include <QAbstractListModel>

#include <sigmafive/resource/resource_manager.hpp>

#include <sigmafive/graphics/context_manager.hpp>

#include <sigmafive/game/entity_manager.hpp>
#include <sigmafive/game/component_manager.hpp>
#include <sigmafive/game/component_system_manager.hpp>

namespace sigmafive {
    namespace editor {
        //https://developer.ubuntu.com/api/apps/qml/sdk-14.10/QtQuick.qtquick-modelviewsdata-cppmodels/
        class EDITOR_API entity_manager_model : public QAbstractListModel {
        Q_OBJECT
        public:
            enum AnimalRoles {
                ID_ROLE = Qt::UserRole + 1
            };

            explicit entity_manager_model(game::entity_manager &entity_manager,
                                          game::component_manager &component_manager,
                                          game::component_system_manager &component_system_manager,
                                          resource::resource_manager &resource_manager,
                                          graphics::context_manager &context_manager,
                                          QObject *parent = nullptr);

            int rowCount(const QModelIndex &parent = QModelIndex()) const;

            QVariant data(const QModelIndex &index, int role) const;

            QHash<int, QByteArray> roleNames() const override {
                QHash<int, QByteArray> roles;
                roles[ID_ROLE] = "ID";
                return roles;
            }

            game::entity_manager *entity_manager() {
                return &entity_manager_;
            }

            game::component_manager *component_manager() {
                return &component_manager_;
            }

            game::component_system_manager *component_system_manager() {
                return &component_system_manager_;
            }

            resource::resource_manager *resource_manager() {
                return &resource_manager_;
            }

            graphics::context_manager *context_manager() {
                return &context_manager_;
            }

        private:
            game::entity_manager &entity_manager_;

            //TODO move to own model
            game::component_manager &component_manager_;
            //TODO move to own model
            game::component_system_manager &component_system_manager_;
            //TODO move to own model
            resource::resource_manager &resource_manager_;
            //TODO move to own model
            graphics::context_manager &context_manager_;
        };
    }
}

#endif //SIGMAFIVE_EDITOR_ENTITY_MANAGER_MODEL_HPP

#ifndef SIGMAFIVE_EDITOR_ASSIMP_IMPORTER_HPP
#define SIGMAFIVE_EDITOR_ASSIMP_IMPORTER_HPP

#include <QObject>
#include <editor/entity_manager_model.hpp>

namespace sigmafive {
    namespace editor {
        class assimp_importer : public QObject {
            Q_OBJECT
        public:
            explicit assimp_importer(QObject* parent = nullptr);

            Q_INVOKABLE void import_file(entity_manager_model *model,QString filepath);
        private:
        };
    }
}

#endif //SIGMAFIVE_EDITOR_ASSIMP_IMPORTER_HPP

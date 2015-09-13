#ifndef SIGMAFIVE_ASSIMP_IMPORT_PLUGIN_HPP
#define SIGMAFIVE_ASSIMP_IMPORT_PLUGIN_HPP

#if defined(_WIN32)
#define ASSIMP_IMPORT_PLUGIN_API __declspec(dllexport)
#else
#define ASSIMP_IMPORT_PLUGIN_API
#endif

#include <QObject>

#include <editor/entity_manager_model.hpp>

class assimp_import_plugin : public QObject {
Q_OBJECT
public:
    explicit assimp_import_plugin(QObject *parent = nullptr);

    Q_INVOKABLE void import_file(sigmafive::editor::entity_manager_model *model, QString filepath);

private:
};


#endif //SIGMAFIVE_ASSIMP_IMPORT_PLUGIN_HPP

#include <editor/application.hpp>

#include <editor/widgets/GameView.hpp>
#include <editor/widgets/MainWindow.hpp>

#include <QQmlContext>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
    sigmafive::editor::application app(argc, argv);

    sigmafive::game::entity_manager entity_manager;
    sigmafive::game::component_manager component_manager(app.component_registry());
    sigmafive::game::component_system_manager component_system_manager;
    component_system_manager.add_component_system<sigmafive::game::static_mesh_component_system>();

    sigmafive::editor::entity_manager_model entity_manager_model(entity_manager,
                                                                 component_manager,
                                                                 component_system_manager,
                                                                 app.graphics_context_manager());

#if 0
    sigmafive::editor::widgets::MainWindow m(&entity_manager_model,&assimp_importer);
    m.show();
#else
    qmlRegisterType<sigmafive::editor::widgets::GameView>("sigmafive.editor.widgets", 1, 0, "GameView");
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("_entityManager", &entity_manager_model);

    engine.load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
#endif

    return app.exec();
}

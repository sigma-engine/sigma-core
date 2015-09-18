#include <editor/application.hpp>
#include <editor/component_manager.hpp>
#include <editor/component_system_manager.hpp>

#include <editor/widgets/GameView.hpp>
#include <editor/widgets/MainWindow.hpp>

#include <sigmafive/game/static_mesh_component_system.hpp>

#include <QQmlContext>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
    sigmafive::editor::application app(argc, argv);

    sigmafive::editor::entity_manager entity_manager;
    sigmafive::editor::component_manager component_manager(app.component_registry());
    sigmafive::editor::component_system_manager component_system_manager;
    component_system_manager.add_component_system<sigmafive::game::static_mesh_component_system>();

#if 0
    sigmafive::editor::widgets::MainWindow m(&entity_manager,
                                             &component_manager,
                                             &component_system_manager,
                                             app.graphics_context_manager(),
                                             &assimp_importer);
    m.show();
#else
    qmlRegisterType<sigmafive::editor::widgets::GameView>("sigmafive.editor.widgets", 1, 0, "GameView");
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("_entityManager", &entity_manager);
    engine.setObjectOwnership(&entity_manager,QQmlEngine::CppOwnership);

    engine.rootContext()->setContextProperty("_componentManager", &component_manager);
    engine.setObjectOwnership(&component_manager,QQmlEngine::CppOwnership);

    engine.rootContext()->setContextProperty("_componentSystemManager", &component_system_manager);
    engine.setObjectOwnership(&component_system_manager,QQmlEngine::CppOwnership);

    engine.load(QUrl(QStringLiteral("qrc:/ui/main.qml")));
#endif

    return app.exec();
}

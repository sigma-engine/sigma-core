#include <editor/application.hpp>
#include <editor/assimp_importer.hpp>
#include <editor/entity_manager_model.hpp>

#include <editor/widgets/MainWindow.hpp>

#include <sigmafive/game/static_mesh_component_system.hpp>

int main(int argc, char *argv[]) {
    sigmafive::editor::application app(argc, argv);

    sigmafive::game::entity_manager entity_manager;
    sigmafive::game::component_manager component_manager(app.component_registry());
    sigmafive::game::component_system_manager component_system_manager;
    component_system_manager.add_component_system<sigmafive::game::static_mesh_component_system>();

    sigmafive::editor::entity_manager_model entity_manager_model(entity_manager,
                                                                 component_manager,
                                                                 component_system_manager,
                                                                 app.resource_manager(),
                                                                 app.graphics_context_manager());
    sigmafive::editor::assimp_importer assimp_importer;

    sigmafive::editor::widgets::MainWindow m(&entity_manager_model,&assimp_importer);
    m.show();

    return app.exec();
}

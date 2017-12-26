#include <package_post_process_effect.hpp>
#include <package_technique.hpp>

#include <sigma/util/filesystem.hpp>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <string>

void package_post_process_effect(
    const sigma::resource::database<sigma::graphics::texture>& texture_database,
    const sigma::resource::database<sigma::graphics::cubemap>& cubemap_database,
    const sigma::resource::database<sigma::graphics::technique>& technique_database,
    const sigma::resource::database<sigma::graphics::static_mesh>& static_mesh_database,
    sigma::resource::database<sigma::graphics::post_process_effect>& post_process_effect_database,
    const boost::filesystem::path& source_directory,
    const boost::filesystem::path& source_file)
{
    auto rid = "post_process_effect" / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
    if (post_process_effect_database.contains({ rid })) {
        auto h = post_process_effect_database.handle_for({ rid });

        auto source_file_time = boost::filesystem::last_write_time(source_file);
        auto resource_time = post_process_effect_database.last_modification_time(h);
        // TODO(NOW): other dependencies
        if (source_file_time <= resource_time)
            return;
    }

    std::cout << "packaging: " << rid << "\n";

    std::ifstream file(source_file.string());

    Json::Value settings;
    file >> settings;

    sigma::graphics::post_process_effect effect;

    convert_effect(texture_database, cubemap_database, technique_database, effect, settings);

    if (settings.isMember("static_mesh"))
        effect.mesh = static_mesh_database.handle_for({ boost::filesystem::path{ "static_mesh" } / settings["static_mesh"].asString() });
    else
        effect.mesh = static_mesh_database.handle_for({ boost::filesystem::path{ "static_mesh/fullscreen_quad" } });

    post_process_effect_database.insert({ rid }, effect);
}

void package_post_process_effects(
    const sigma::resource::database<sigma::graphics::texture>& texture_database,
    const sigma::resource::database<sigma::graphics::cubemap>& cubemap_database,
    const sigma::resource::database<sigma::graphics::technique>& technique_database,
    const sigma::resource::database<sigma::graphics::static_mesh>& static_mesh_database,
    sigma::resource::database<sigma::graphics::post_process_effect>& post_process_effect_database,
    const boost::filesystem::path& source_directory)
{
    boost::filesystem::recursive_directory_iterator it{ source_directory };
    boost::filesystem::recursive_directory_iterator end;
    for (; it != end; ++it) {
        auto path = it->path();
        if (sigma::filesystem::is_hidden(path)) {
            if (boost::filesystem::is_directory(path))
                it.no_push();
            continue;
        }

        auto ext = boost::algorithm::to_lower_copy(path.extension().string());
        if (boost::filesystem::is_regular_file(path) && ext == ".eff") {
            try {
                package_post_process_effect(texture_database, cubemap_database, technique_database, static_mesh_database, post_process_effect_database, source_directory, path);
            } catch (const std::runtime_error& e) {
                std::cerr << "error: " << e.what() << '\n';
            }
        }
    }
}

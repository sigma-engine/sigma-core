#include <package_material.hpp>
#include <package_technique.hpp>

#include <sigma/util/filesystem.hpp>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <string>

void package_material(
    const sigma::resource::database<sigma::graphics::texture>& texture_database,
    const sigma::resource::database<sigma::graphics::cubemap>& cubemap_database,
    const sigma::resource::database<sigma::graphics::technique>& technique_database,
    sigma::resource::database<sigma::graphics::material>& material_database,
    const boost::filesystem::path& source_directory,
    const boost::filesystem::path& source_file)
{
    auto rid = "material" / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
    if (material_database.contains({ rid })) {
        auto h = material_database.handle_for({ rid });

        auto source_file_time = boost::filesystem::last_write_time(source_file);
        auto resource_time = material_database.last_modification_time(h);
        // TODO(NOW): other dependencies
        if (source_file_time <= resource_time)
            return;
    }

    std::cout << "packaging: " << rid << "\n";

    std::ifstream file(source_file.string());

    Json::Value settings;
    file >> settings;

    sigma::graphics::material material;

    convert_effect(texture_database, cubemap_database, technique_database, material, settings);

    material_database.insert({ rid }, material);
}

void package_materials(
    const sigma::resource::database<sigma::graphics::texture>& texture_database,
    const sigma::resource::database<sigma::graphics::cubemap>& cubemap_database,
    const sigma::resource::database<sigma::graphics::technique>& technique_database,
    sigma::resource::database<sigma::graphics::material>& material_database,
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
        if (boost::filesystem::is_regular_file(path) && ext == ".smat") {
            try {
                package_material(texture_database, cubemap_database, technique_database, material_database, source_directory, path);
            } catch (const std::runtime_error& e) {
                std::cerr << "error: " << e.what() << '\n';
            }
        }
    }
}

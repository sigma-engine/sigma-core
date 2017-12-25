#include <package_cubemap.hpp>

#include <sigma/util/filesystem.hpp>

#include <boost/algorithm/string.hpp>

#include <string>

void package_cubemap(
    const sigma::resource::database<sigma::graphics::texture>& texture_database,
    sigma::resource::database<sigma::graphics::cubemap>& cubemap_database,
    const boost::filesystem::path& source_directory,
    const boost::filesystem::path& source_file)
{
    auto rid = "cubemap" / sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
    auto h = cubemap_database.handle_for({ rid });

    if (h.is_valid()) {
        auto source_file_time = boost::filesystem::last_write_time(source_file);
        auto resource_time = cubemap_database.last_modification_time(h);
        // TODO(NOW): other dependencies
        if (source_file_time <= resource_time)
            return;
    }

    std::cout << "packaging: " << rid << "\n";

    Json::Value settings;

    std::ifstream file(source_file.string());
    file >> settings;

    sigma::graphics::cubemap cubemap;
    // TODO:(NOW) error if can not find textures.
    // TODO: (NOW) throw if missing a face.
    cubemap.right = texture_database.handle_for({ boost::filesystem::path{ "texture" } / settings["right"].asString() });
    cubemap.left = texture_database.handle_for({ boost::filesystem::path{ "texture" } / settings["left"].asString() });
    cubemap.top = texture_database.handle_for({ boost::filesystem::path{ "texture" } / settings["top"].asString() });
    cubemap.bottom = texture_database.handle_for({ boost::filesystem::path{ "texture" } / settings["bottom"].asString() });
    cubemap.back = texture_database.handle_for({ boost::filesystem::path{ "texture" } / settings["back"].asString() });
    cubemap.front = texture_database.handle_for({ boost::filesystem::path{ "texture" } / settings["front"].asString() });
    cubemap_database.insert({ rid }, cubemap);
}

void package_cubemaps(
    const sigma::resource::database<sigma::graphics::texture>& texture_database,
    sigma::resource::database<sigma::graphics::cubemap>& cubemap_database,
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
        if (boost::filesystem::is_regular_file(path) && ext == ".cub")
            package_cubemap(texture_database, cubemap_database, source_directory, path);
    }
}

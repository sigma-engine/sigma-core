#include <package_technique.hpp>

#include <sigma/util/filesystem.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <string>
#include <iostream>

void package_technique(
    const sigma::resource::database<sigma::graphics::shader>& shader_database,
    sigma::resource::database<sigma::graphics::technique>& technique_database,
    const boost::filesystem::path& source_directory,
    const std::vector<boost::filesystem::path>& include_directories,
    const boost::filesystem::path& source_file)
{
    std::ifstream file(source_file.string());

    Json::Value settings;
    file >> settings;

    sigma::graphics::technique technique;
    std::vector<boost::filesystem::path> cid;
    // TODO error if missing any shaders.
    if (settings.isMember("vertex")) {
        cid.push_back(boost::filesystem::path{ "vertex" } / settings["vertex"].asString());
        technique.shaders.vertex = shader_database.handle_for({ cid.back() });
    }
    if (settings.isMember("tessellation_control")) {
        cid.push_back(boost::filesystem::path{ "tessellation_control" } / settings["tessellation_control"].asString());
        technique.shaders.tessellation_control = shader_database.handle_for({ cid.back() });
    }
    if (settings.isMember("tessellation_evaluation")) {
        cid.push_back(boost::filesystem::path{ "tessellation_evaluation" } / settings["tessellation_evaluation"].asString());
        technique.shaders.tessellation_evaluation = shader_database.handle_for({ cid.back() });
    }
    if (settings.isMember("geometry")) {
        cid.push_back(boost::filesystem::path{ "geometry" } / settings["geometry"].asString());
        technique.shaders.geometry = shader_database.handle_for({ cid.back() });
    }
    if (settings.isMember("fragment")) {
        cid.push_back(boost::filesystem::path{ "fragment" } / settings["fragment"].asString());
        technique.shaders.fragment = shader_database.handle_for({ cid.back() });
    }


    if (technique_database.contains(cid)) {
        auto h = technique_database.handle_for(cid);

        auto source_file_time = boost::filesystem::last_write_time(source_file);
        auto resource_time = technique_database.last_modification_time(h);
        // TODO(NOW): other dependencies
        if (source_file_time <= resource_time)
            return;
    }

    std::cout << "packaging: technique { " << boost::algorithm::join(cid | boost::adaptors::transformed([](auto a) { return a.string(); }), ", ") << " }\n";

    // TODO: (NOW) check for link errors.
    // TODO: (NOW) extract reflection data.

    technique_database.insert(cid, technique);
}

void package_techniques(
    const sigma::resource::database<sigma::graphics::shader>& shader_database,
    sigma::resource::database<sigma::graphics::technique>& technique_database,
    const boost::filesystem::path& source_directory,
    const std::vector<boost::filesystem::path>& include_directories)
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
        if (boost::filesystem::is_regular_file(path) && (ext == ".stech" || ext == ".smat" || ext == ".eff"))
            package_technique(shader_database, technique_database, source_directory, include_directories, path);
    }
}

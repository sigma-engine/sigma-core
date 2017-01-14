#include <compiler_util.hpp>

#include <sigma/util/filesystem.hpp>

#include <boost/filesystem/operations.hpp>

#include <fstream>

namespace sigma {
void touch_stamp_file(boost::filesystem::path outputdir, boost::filesystem::path resource, const std::vector<boost::filesystem::path>& dependencies)
{
    auto rel_resource = sigma::filesystem::make_relative(boost::filesystem::current_path(), resource);
    auto stamp_path = outputdir / rel_resource;
    stamp_path += ".stamp";
    boost::filesystem::create_directories(stamp_path.parent_path());
    std::ofstream stamp_file(stamp_path.string());
    stamp_file << std::chrono::high_resolution_clock::now().time_since_epoch().count();

    for (const auto& dep : dependencies)
        stamp_file << dep.string() << std::endl;
}

bool resource_has_changes(boost::filesystem::path outputdir, boost::filesystem::path resource)
{
    auto rel_resource = sigma::filesystem::make_relative(boost::filesystem::current_path(), resource);
    auto stamp_path = outputdir / rel_resource;
    stamp_path += ".stamp";

    if (!boost::filesystem::exists(stamp_path))
        return true;

    auto stamp_time = boost::filesystem::last_write_time(stamp_path);
    std::ifstream stamp_file(stamp_path.string());

    std::size_t t;
    stamp_file >> t;

    std::string dep;
    while (std::getline(stamp_file, dep)) {
        if (stamp_file.good() && stamp_time < boost::filesystem::last_write_time(dep))
            return true;
    }

    return boost::filesystem::last_write_time(stamp_path) < boost::filesystem::last_write_time(resource);
}
}

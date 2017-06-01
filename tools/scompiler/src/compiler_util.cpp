#include <compiler_util.hpp>

#include <sigma/util/filesystem.hpp>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include <fstream>
#include <string>

namespace sigma {
boost::filesystem::path stamp_file_path(const boost::filesystem::path& outputdir, const boost::filesystem::path& resource)
{
    auto rel_resource = sigma::filesystem::make_relative(boost::filesystem::current_path(), resource);
    auto stamp_path = outputdir / rel_resource;
    stamp_path += ".stamp";
    return stamp_path;
}

void touch_stamp_file(const boost::filesystem::path& outputdir, const boost::filesystem::path& resource, const std::vector<boost::filesystem::path>& dependencies)
{
    auto stamp_path = stamp_file_path(outputdir, resource);

    boost::filesystem::create_directories(stamp_path.parent_path());
    std::ofstream stamp_file(stamp_path.string());

    for (const auto& dep : dependencies)
        stamp_file << dep.string() << ';';
}
}

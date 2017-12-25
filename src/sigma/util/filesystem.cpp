#include <sigma/util/filesystem.hpp>

#include <boost/filesystem/path.hpp>

namespace sigma {
namespace filesystem {
    bool contains_file(boost::filesystem::path directory, boost::filesystem::path file)
    {
        auto dbegin = directory.begin();
        auto dend = directory.end();
        auto fbegin = file.begin();
        auto fend = file.end();

        auto directory_size = std::distance(dbegin, dend);
        auto file_size = std::distance(fbegin, fend);
        if (directory_size > file_size)
            return false;

        return std::equal(dbegin, dend, fbegin);
    }

    boost::filesystem::path make_relative(boost::filesystem::path directory, boost::filesystem::path file)
    {
        auto dbegin = directory.begin();
        auto fbegin = file.begin();
        auto fend = file.end();

        while (*dbegin == *fbegin) {
            dbegin++;
            fbegin++;
        }
        boost::filesystem::path output;
        do {
            output = output / (*fbegin).string();
            fbegin++;
        } while (fbegin != fend);
        return output;
    }

    bool is_hidden(const boost::filesystem::path& path)
    {
        auto name = path.filename().string();
        return !name.empty() && name != ".." && name != "." && name[0] == '.';
    }
}
}

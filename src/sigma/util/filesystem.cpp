#include <sigma/util/filesystem.hpp>

#include <filesystem>

namespace sigma {
namespace filesystem {
    bool contains_file(std::filesystem::path directory, std::filesystem::path file)
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

    std::filesystem::path make_relative(std::filesystem::path directory, std::filesystem::path file)
    {
        auto dbegin = directory.begin();
        auto dend = directory.end();
        auto fbegin = file.begin();
        auto fend = file.end();

        while (dbegin != dend && fbegin != fend && *dbegin == *fbegin) {
            dbegin++;
            fbegin++;
        }
        std::filesystem::path output;
        do {
            output = output / (*fbegin).string();
            fbegin++;
        } while (fbegin != fend);
        return output;
    }

    bool is_hidden(const std::filesystem::path& path)
    {
        auto name = path.filename().string();
        return !name.empty() && name != ".." && name != "." && name[0] == '.';
    }
}
}

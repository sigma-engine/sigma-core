#include <sigma/util/filesystem.hpp>

namespace sigma {
namespace util {
    bool directory_contains_file(boost::filesystem::path directory, boost::filesystem::path file)
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

    boost::filesystem::path path_divergence(boost::filesystem::path directory, boost::filesystem::path file)
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
}
}

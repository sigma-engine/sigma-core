#ifndef SIGMA_ENGINE_FILESYSTEM_FILESYSTEM_HPP
#define SIGMA_ENGINE_FILESYSTEM_FILESYSTEM_HPP

#include <sigma/config.hpp>

#include <cereal/types/string.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/functional/hash.hpp>

namespace sigma {
namespace filesystem {
    bool SIGMA_API contains_file(boost::filesystem::path directory, boost::filesystem::path file);

    boost::filesystem::path SIGMA_API make_relative(boost::filesystem::path directory, boost::filesystem::path file);

    bool SIGMA_API is_hidden(const boost::filesystem::path& path);
}
}

namespace std {
template <>
struct hash<boost::filesystem::path> {
    size_t operator()(const boost::filesystem::path& p) const
    {
        return boost::filesystem::hash_value(p);
    }
};
}

namespace cereal {
template <class Archive>
inline void save(Archive& ar, const boost::filesystem::path& p)
{
    auto str = p.string();
    ar(str);
}

template <class Archive>
inline void load(Archive& ar, boost::filesystem::path& p)
{
    boost::filesystem::path::string_type str;
    ar(str);
    p = str;
}
}

#endif // SIGMA_ENGINE_FILESYSTEM_FILESYSTEM_HPP

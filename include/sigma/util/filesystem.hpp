#ifndef SIGMA_ENGINE_FILESYSTEM_FILESYSTEM_HPP
#define SIGMA_ENGINE_FILESYSTEM_FILESYSTEM_HPP

#include <sigma/config.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/functional/hash.hpp>
#include <boost/serialization/level.hpp>
#include <boost/serialization/nvp.hpp>

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

namespace boost {
namespace serialization {
    template <class Archive>
    void serialize(Archive& ar, boost::filesystem::path& p, const unsigned int version)
    {
        boost::filesystem::path::string_type s;
        if (Archive::is_saving::value)
            s = p.string();
        ar& boost::serialization::make_nvp("string", s);
        if (Archive::is_loading::value)
            p = s;
    }
}
}

#endif // SIGMA_ENGINE_FILESYSTEM_FILESYSTEM_HPP

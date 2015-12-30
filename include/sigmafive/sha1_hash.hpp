#ifndef SIGMAFIVE_SHA1_HASH_HPP
#define SIGMAFIVE_SHA1_HASH_HPP

#include <array>
#include <boost/filesystem/path.hpp>

namespace sigmafive {
    struct sha1_hash {
    public:
        sha1_hash(std::string sha1);

        std::string string() const;

        boost::filesystem::path path() const;

        bool operator==(const sha1_hash &other) const;

        bool operator!=(const sha1_hash &other) const;
    private:
        std::array<unsigned char,20> data;
    };
}

#endif //SIGMAFIVE_SHA1_HASH_HPP
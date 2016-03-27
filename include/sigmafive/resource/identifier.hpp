#ifndef SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP
#define SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP

#include <sigmafive/config.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/serialization/access.hpp>
#include <functional>
#include <iostream>
#include <sigmafive/util/compile_time_hash.hpp>
#include <sigmafive/util/filesystem.hpp>

namespace sigmafive {
namespace resource {
    struct SIGMAFIVE_API identifier {
        identifier();

        identifier(std::string type, boost::filesystem::path path, boost::filesystem::path root_directroy = boost::filesystem::current_path());

        identifier(std::string type, boost::filesystem::path path, std::string sub_name, boost::filesystem::path root_directroy = boost::filesystem::current_path());

        identifier(const std::string& name);

        identifier(const char* name);

        identifier(const identifier& other);

        bool operator==(const identifier& other) const noexcept;

        bool operator!=(const identifier& other) const noexcept;

        util::hash_type value() const noexcept;

        std::string name() const;

        explicit operator util::hash_type() const noexcept;

    private:
        util::hash_type value_;
        std::string name_;

        friend class boost::serialization::access;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& value_;
            ar& name_;
        }
    };

    SIGMAFIVE_API std::ostream& operator<<(std::ostream& os, const identifier& id);
}
}

namespace std {
template <>
struct hash<sigmafive::resource::identifier> {
    sigmafive::util::hash_type operator()(const sigmafive::resource::identifier& id) const { return static_cast<sigmafive::util::hash_type>(id); }
};
}

#endif //SIGMAFIVE_ENGINE_RESOURCE_IDENTIFIER_HPP

#ifndef SIGMA_RESOURCE_IDENTIFIER_HPP
#define SIGMA_RESOURCE_IDENTIFIER_HPP

#include <sigma/config.hpp>
#include <sigma/util/compile_time_hash.hpp>

#include <json/json.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/serialization/access.hpp>

#include <cstddef>
#include <functional>
#include <ostream>
#include <string>

namespace sigma {

// TODO:RESOURCE remove this when the new resource system is in place
namespace resource {
    struct identifier;
}
namespace json {
    // TODO:RESOURCE remove this when the new resource system is in place
    static bool from_json(const Json::Value& source, resource::identifier& output);

    // TODO:RESOURCE remove this when the new resource system is in place
    static void to_json(const resource::identifier& source, Json::Value& output);
}

namespace resource {
    struct SIGMA_API identifier {
    public:
        identifier();

        identifier(const char* name);

        identifier(const std::string& name);

        identifier(std::string type, boost::filesystem::path path, boost::filesystem::path root_directroy = boost::filesystem::current_path());

        identifier(std::string type, boost::filesystem::path path, std::string sub_name, boost::filesystem::path root_directroy = boost::filesystem::current_path());

        template <std::size_t N>
        identifier(const char (&name)[N])
            : value_(util::compile_time_hash(name))
            , name_(name)
        {
        }

        bool operator==(const identifier& other) const noexcept;

        bool operator!=(const identifier& other) const noexcept;

        util::hash_type value() const noexcept;

        explicit operator util::hash_type() const noexcept;

        std::string name() const;

    protected:
        util::hash_type value_;
        std::string name_;

        friend class boost::serialization::access;

        // TODO:RESOURCE remove this when the new resource system is in place
        friend bool json::from_json(const Json::Value& source, identifier& output);
        friend void json::to_json(const identifier& source, Json::Value& output);

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& value_;
            ar& name_;
        }
    };

    SIGMA_API std::ostream& operator<<(std::ostream& os, const identifier& id);

    using identifier = identifier;
}
namespace json {
    // TODO:RESOURCE remove this when the new resource system is in place
    static bool from_json(const Json::Value& source, resource::identifier& output)
    {
        output.name_ = source.asString();
        output.value_ = util::compile_time_hash(output.name_.c_str());
        return true;
    }

    // TODO:RESOURCE remove this when the new resource system is in place
    static void to_json(const resource::identifier& source, Json::Value& output)
    {
        output = source.name_;
    }
}
}

namespace std {
template <>
struct hash<sigma::resource::identifier> {
    sigma::util::hash_type operator()(const sigma::resource::identifier& id) const
    {
        return static_cast<sigma::util::hash_type>(id);
    }
};
}

#endif // SIGMA_RESOURCE_IDENTIFIER_HPP

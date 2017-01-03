#include <sigma/resource/identifier.hpp>
#include <sigma/util/compile_time_hash.hpp>
#include <sigma/util/filesystem.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/serialization/access.hpp>

#include <functional>
#include <iostream>

namespace sigma {
namespace resource {
    bool constexpr_identifier::operator==(const constexpr_identifier& other) const
        noexcept
    {
        return value_ == other.value_;
    }

    bool constexpr_identifier::operator!=(const constexpr_identifier& other) const
        noexcept
    {
        return value_ != other.value_;
    }

    util::hash_type constexpr_identifier::value() const noexcept { return value_; }

    constexpr_identifier::operator util::hash_type() const noexcept
    {
        return value_;
    }

    development_identifier::development_identifier(const char* name)
        : constexpr_identifier(util::compile_time_hash(name))
        , name_(name)
    {
    }

    development_identifier::development_identifier(
        std::string type, boost::filesystem::path path,
        boost::filesystem::path root_directroy)
    {
        // TODO error if path is not in root_directroy
        path = boost::filesystem::absolute(path, root_directroy);
        path = path.replace_extension("");
        auto resource_path = filesystem::make_relative(root_directroy, path);
        name_ = resource_path.string();
        boost::algorithm::replace_all(name_, "\\", "/");
        name_ = type + "://" + name_;
        value_ = util::compile_time_hash(name_.c_str());
    }

    development_identifier::development_identifier(
        std::string type, boost::filesystem::path path, std::string sub_name,
        boost::filesystem::path root_directroy)
    {
        // TODO error if path is not in root_directroy
        path = boost::filesystem::absolute(path, root_directroy);
        path = path.replace_extension("");
        auto resource_path = filesystem::make_relative(root_directroy, path);
        name_ = resource_path.string() + ":" + sub_name;
        boost::algorithm::replace_all(name_, "\\", "/");
        name_ = type + "://" + name_;
        value_ = util::compile_time_hash(name_.c_str());
    }

    development_identifier::development_identifier(const std::string& name)
        : constexpr_identifier(util::compile_time_hash(name.c_str()))
        , name_(name)
    {
    }

    std::string development_identifier::name() const { return name_; }

    std::string development_identifier::nice_name() const
    {
        auto s = boost::find_first(name_, "://").end();
        return std::string(s, name_.end());
    }

    std::ostream& operator<<(std::ostream& os, const constexpr_identifier& id)
    {
        os << "{"
           << "\"value:\"" << id.value() << "}";
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const development_identifier& id)
    {
        os << "{"
           << "\"value:\"" << id.value() << ",\"name:\"" << id.name() << "}";
        return os;
    }
}
}

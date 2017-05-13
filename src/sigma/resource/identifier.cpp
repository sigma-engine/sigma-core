#include <sigma/resource/identifier.hpp>
#include <sigma/util/compile_time_hash.hpp>
#include <sigma/util/filesystem.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/serialization/access.hpp>

#include <functional>
#include <iostream>
#include <random>

namespace sigma {
namespace resource {

    identifier::identifier()
        : value_(-1)
    {
    }

    identifier::identifier(const char* name)
        : value_(util::compile_time_hash(name))
        , name_(name)
    {
    }

    identifier::identifier(const std::string& name)
        : value_(util::compile_time_hash(name.c_str()))
        , name_(name)
    {
    }

    identifier::identifier(std::string type, boost::filesystem::path path, boost::filesystem::path root_directroy)
    {
        // TODO error if path is not in root_directroy
        path = boost::filesystem::absolute(path, root_directroy);
        path = path.replace_extension("");
        auto resource_path = filesystem::make_relative(root_directroy, path);
        name_ = resource_path.string();
        boost::algorithm::replace_all(name_, "\\", "/");

        auto s = boost::find_first(name_, "://").end();
        if (s != name_.end())
            name_ = std::string(s, name_.end());

        name_ = type + "://" + name_;
        value_ = util::compile_time_hash(name_.c_str());
    }

    identifier::identifier(std::string type, boost::filesystem::path path, std::string sub_name, boost::filesystem::path root_directroy)
    {
        // TODO error if path is not in root_directroy
        path = boost::filesystem::absolute(path, root_directroy);
        path = path.replace_extension("");
        auto resource_path = filesystem::make_relative(root_directroy, path);
        name_ = resource_path.string() + ":" + sub_name;
        boost::algorithm::replace_all(name_, "\\", "/");

        auto s = boost::find_first(name_, "://").end();
        if (s != name_.end())
            name_ = std::string(s, name_.end());

        name_ = type + "://" + name_;
        value_ = util::compile_time_hash(name_.c_str());
    }

    bool identifier::operator==(const identifier& other) const noexcept
    {
        return value_ == other.value_;
    }

    bool identifier::operator!=(const identifier& other) const noexcept
    {
        return value_ != other.value_;
    }

    util::hash_type identifier::value() const noexcept { return value_; }

    identifier::operator util::hash_type() const noexcept
    {
        return value_;
    }

    std::string identifier::name() const { return name_; }

    std::ostream& operator<<(std::ostream& os, const identifier& id)
    {
        os << "{"
           << "value: " << id.value() << ", name: " << id.name() << "}";
        return os;
    }
}
}

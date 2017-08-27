#ifndef SIGMA_ENGINE_RESOURCE_DATABASE_HPP
#define SIGMA_ENGINE_RESOURCE_DATABASE_HPP

#include <sigma/config.hpp>

#include <sigma/resource/cache.hpp>

#include <json/json.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/functional/hash.hpp>
#include <boost/interprocess/sync/file_lock.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include <fstream>
#include <string>

namespace sigma {
namespace resource {

    template <class Resource>
    class database {
    public:
        database(const boost::filesystem::path& cache_directory, const std::string& resource_type)
            : storage_directory_(cache_directory / resource_type)
            , database_file_(storage_directory_ / "database.json")
            , mutex_((storage_directory_ / "database.lock").c_str())
        {
            // Create the database file if it does not exist.
            if (!boost::filesystem::exists(database_file_)) {
                // Lock the database file.
                boost::interprocess::scoped_lock<mutex_type> sol(mutex_);
                std::ofstream dbfile(database_file_.string());
                dbfile << "{\n\"next_handle\": 0\n}\n";
            }
        }

        handle<Resource> handle_for(const std::vector<boost::filesystem::path>& cid) const
        {
            handle<Resource> h;
            std::size_t hash_code = 0;
            for (const auto& id : cid)
                boost::hash_combine(hash_code, id);
            auto hash_code_string = std::to_string(hash_code);

            // Read in the current database.
            Json::Value json(Json::objectValue);
            {
                // Lock the database file.
                boost::interprocess::scoped_lock<mutex_type> sol(mutex_);
                std::ifstream file(database_file_.string());
                file >> json;
            }

            if (json.isMember(hash_code_string)) {
                auto& json_cid = json[hash_code_string]["cid"];
                assert(std::equal(cid.begin(), cid.end(), json_cid.begin(), json_cid.end(), [](auto id, auto jid) {
                    return id == jid.asString();
                }) /*resource id hash collision.*/);
                h.index = json[hash_code_string]["index"].asInt();
                // TODO version
                h.version = 0;
            }

            return h;
        }

        handle<Resource> insert(const std::vector<boost::filesystem::path>& cid, Resource& res)
        {
            handle<Resource> h;
            std::size_t hash_code = 0;
            for (const auto& id : cid)
                boost::hash_combine(hash_code, id);
            auto hash_code_string = std::to_string(hash_code);

            {
                // Lock the database file.
                boost::interprocess::scoped_lock<mutex_type> sol(mutex_);

                // Read in the current database.
                Json::Value json(Json::objectValue);
                {
                    std::ifstream file(database_file_.string());
                    file >> json;
                }

                if (json.isMember(hash_code_string)) {
                    auto& json_cid = json[hash_code_string]["cid"];
                    assert(std::equal(cid.begin(), cid.end(), json_cid.begin(), json_cid.end(), [](auto id, auto jid) {
                        return id == jid.asString();
                    }) /*resource id hash collision.*/);
                    h.index = json[hash_code_string]["index"].asInt();
                    // TODO version
                    h.version = 0;
                } else {
                    h.index = json["next_handle"].asInt();
                    // TODO version
                    h.version = 0;

                    json[hash_code_string]["index"] = h.index;
                    for (const auto& id : cid)
                        json[hash_code_string]["cid"].append(id.string());
                    json["next_handle"] = h.index + 1;

                    std::ofstream file(database_file_.string());
                    file << json;
                }
            }

            std::ofstream stream{ (storage_directory_ / std::to_string(h.index)).string(), std::ios::binary };
            boost::archive::binary_oarchive oa(stream);
            oa << res;

            return h;
        }

    private:
        database(const database<Resource>&) = delete;
        database<Resource>& operator=(const database<Resource>&) = delete;

        boost::filesystem::path storage_directory_;
        boost::filesystem::path database_file_;
        using mutex_type = boost::interprocess::file_lock;
        mutable mutex_type mutex_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_DATABASE_HPP

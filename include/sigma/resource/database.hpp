#ifndef SIGMA_ENGINE_RESOURCE_DATABASE_HPP
#define SIGMA_ENGINE_RESOURCE_DATABASE_HPP

#include <sigma/config.hpp>
#include <sigma/resource/cache.hpp>

#include <json/json.h>

#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/functional/hash.hpp>

#include <fstream>
#include <string>
#include <iostream>

namespace sigma {
namespace resource {

    template <class Resource>
    class database {
    public:
        database(const boost::filesystem::path& cache_directory, const std::string& resource_type)
            : storage_directory_(cache_directory / resource_type)
            , database_file_(storage_directory_ / "database.json")
        {
            if (!boost::filesystem::exists(storage_directory_))
                boost::filesystem::create_directories(storage_directory_);

            // Create the database file if it does not exist.
            if (!boost::filesystem::exists(database_file_)) {
                std::ofstream dbfile(database_file_.string());
                dbfile << "{\n\"next_handle\": 0\n}\n";
            }
        }

        std::size_t hash_code_for(const std::vector<boost::filesystem::path>& cid) const
        {
            std::size_t hash_code = 0;
            for (const auto& id : cid)
                boost::hash_combine(hash_code, id);
            return hash_code;
        }

        bool contains(const std::vector<boost::filesystem::path>& cid) const {
            std::size_t hash_code = hash_code_for(cid);
            auto hash_code_string = std::to_string(hash_code);

            // Read in the current database.
            Json::Value json(Json::objectValue);
            std::ifstream file(database_file_.string());
            file >> json;

            return json.isMember(hash_code_string);
        }

        handle<Resource> handle_for(const std::vector<boost::filesystem::path>& cid) const
        {
            handle<Resource> h;
            std::size_t hash_code = hash_code_for(cid);
            auto hash_code_string = std::to_string(hash_code);

            // Read in the current database.
            Json::Value json(Json::objectValue);
            std::ifstream file(database_file_.string());
            file >> json;

            if (json.isMember(hash_code_string)) {
                auto& json_cid = json[hash_code_string]["cid"];
                assert(std::equal(cid.begin(), cid.end(), json_cid.begin(), json_cid.end(), [](auto id, auto jid) {
                    return id == jid.asString();
                }) /*resource id hash collision.*/);
                h.index = json[hash_code_string]["index"].asInt();
                // TODO version
                h.version = 0;
            }
            else {
                std::cout << "missing :";
                for(auto path: cid) {
                    std::cout << path;
                }
                std::cout << std::endl;
            }

            return h;
        }

        std::time_t last_modification_time(const handle<Resource>& h) const
        {
            return boost::filesystem::last_write_time((storage_directory_ / std::to_string(h.index)).string());
        }

        handle<Resource> insert(const std::vector<boost::filesystem::path>& cid, Resource& res)
        {
            handle<Resource> h;
            std::size_t hash_code = hash_code_for(cid);
            auto hash_code_string = std::to_string(hash_code);

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
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_DATABASE_HPP

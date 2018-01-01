#ifndef SIGMA_TOOLS_RESOURCE_HASH_HPP
#define SIGMA_TOOLS_RESOURCE_HASH_HPP

#include <boost/filesystem/path.hpp>
#include <boost/functional/hash.hpp>

#include <vector>

namespace sigma {
namespace tools {
    using complex_resource_id = std::vector<boost::filesystem::path>;

    std::size_t resource_id_for(const complex_resource_id& cid)
    {
        std::size_t hash_code = 0;
        for (const auto& id : cid) {
            if (id.size() > 0)
                boost::hash_combine(hash_code, id);
        }
        return hash_code;
    }
}
}

#endif // SIGMA_TOOLS_RESOURCE_HASH_HPP

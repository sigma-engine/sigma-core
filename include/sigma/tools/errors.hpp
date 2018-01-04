#ifndef SIGMA_TOOLS_ERRORS_HPP
#define SIGMA_TOOLS_ERRORS_HPP

#include <sigma/tools/resource_hash.hpp>

#include <exception>
#include <sstream>

namespace sigma {
namespace tools {
    class missing_resource : public std::exception {
    public:
        missing_resource(const complex_resource_id& cid)
        {
            std::stringstream ss;

            ss << "missing resource ";
            if (cid.size() >= 2) {
                ss << "{ " << cid[0];
                for (std::size_t i = 1; i < cid.size() - 1; ++i) {
                    if (cid[i].size() > 0)
                        ss << cid[i] << ", ";
                }
                ss << cid[cid.size() - 1] << "}";
            } else if (cid.size() == 1) {
                ss << cid[0];
            } else {
                ss << "<UNKNOWN>";
            }
            message_ = ss.str();
        }

        virtual const char* what() const noexcept override
        {
            return message_.c_str();
        }

    private:
        std::string message_;
    };
}
}

#endif // SIGMA_TOOLS_ERRORS_HPP

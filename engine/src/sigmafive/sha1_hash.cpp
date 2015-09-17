#include <sigmafive/sha1_hash.hpp>

#include <sstream>

namespace sigmafive {
    sha1_hash::sha1_hash(std::string sha1) {
        std::stringstream ss;
        for(unsigned int i=0;i<data.size();++i) {
            int x;
            ss << sha1.at(2*i) << sha1.at(2*i+1);
            ss >> std::hex >> x;
            ss.clear();
            data[i] = (unsigned char)x;
        }
    }

    std::string sha1_hash::string() const {
        std::stringstream ss;
        for (int i = 0; i < data.size(); i++) {
            ss << std::hex << (data.at(i) >> 4);
            ss << std::hex << (data.at(i) & 0xf);
        }
        return ss.str();
    }

    boost::filesystem::path sha1_hash::path() const {
        auto str = string();
        return boost::filesystem::path{str.substr(0,2)}/str.substr(2,38);
    }


    bool sha1_hash::operator==(const sha1_hash &other) const {
        return std::equal(data.begin(),data.end(),other.data.begin());
    }

    bool sha1_hash::operator!=(const sha1_hash &other) const {
        return !std::equal(data.begin(),data.end(),other.data.begin());
    }
}

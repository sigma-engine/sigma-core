#ifndef SIGMAFIVE_RESOURCE_RESOURCE_HPP
#define SIGMAFIVE_RESOURCE_RESOURCE_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>

#include <boost/filesystem/path.hpp>

namespace sigmafive {
    namespace resource {

        class identifier {
        public:
            identifier(boost::filesystem::path path);

            boost::filesystem::path path() const;

            std::size_t hash() const;

            bool operator ==(const identifier &other) const;

            bool operator !=(const identifier &other) const;
        private:
            boost::filesystem::path path_;
            std::size_t hash_;
        };

        class SIGMAFIVE_API resource : public cppbr::meta::object {
        CPPBR_META_CLASS()
        public:
            resource(identifier id);

            virtual ~resource() = default;

            identifier id() const;
        private:
            identifier id_;
        };
    }
}

namespace std {
    template<>
    struct hash<sigmafive::resource::identifier> {
        std::size_t operator ()(const sigmafive::resource::identifier &id) const noexcept {
            return id.hash();
        }
    };
}

#endif //SIGMAFIVE_RESOURCE_RESOURCE_HPP

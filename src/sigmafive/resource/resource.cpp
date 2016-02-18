#include <sigmafive/resource/resource.hpp>
#include <boost/uuid/random_generator.hpp>

namespace sigmafive {
    namespace resource {
        identifier::identifier(boost::filesystem::path path)
            : path_(path), hash_(cppbr::meta::compile_time_hash(path.string().c_str())) {
        }

        boost::filesystem::path identifier::path() const {
            return path_;
        }

        std::size_t identifier::hash() const {
            return hash_;
        }

        bool identifier::operator ==(const identifier &other) const {
            return hash_ == other.hash_;
        }

        bool identifier::operator !=(const identifier &other) const {
            return hash_ != other.hash_;
        }

        resource::resource(identifier id) : id_(id) {
        }

        identifier resource::id() const {
            return id_;
        }
    }
}

EXPORT_CPPBR_META_CLASS(sigmafive::resource::resource)

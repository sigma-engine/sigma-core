#include <sigma/resource/resource.hpp>

namespace sigma {
namespace resource {

    base_resource::base_resource(std::weak_ptr<sigma::context> context, const key_type& key)
        : context_(context)
        , key_(key)
    {
    }

    base_resource::~base_resource() {}

    std::weak_ptr<sigma::context> base_resource::context()
    {
        return context_;
    }

    const key_type& base_resource::key() const
    {
        return key_;
    }

    size_t base_resource::id() const
    {
        return id_;
    }

    void base_resource::set_id(size_t id)
    {
        id_ = id;
    }
}
}

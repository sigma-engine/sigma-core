#ifndef SIGMA_ENGINE_RESOURCE_MANAGER_HPP
#define SIGMA_ENGINE_RESOURCE_MANAGER_HPP

#include <sigma/resource/resource_cache.hpp>

namespace sigma {
namespace resource {

    template <typename PublicType, typename InteralType>
    class resource_manager {
    public:
        resource_manager(resource_cache<PublicType>& cache)
            : cache_(cache)
        {
        }

		std::shared_ptr<InteralType> get_interal(resource::identifier resource_id)
		{
			// TODO this not very clean or safe
			cache_.increment_reference(resource_id);
			auto r = get(resource_id);
			cache_.decrement_reference(resource_id);
			return r;
		}

        std::shared_ptr<InteralType> get(resource::identifier resource_id)
        {
            // TODO what should happen when missing resources!
            assert(cache_.is_loaded(resource_id) && "Resource missing!");

            auto it = internals_.find(resource_id);
            if (it != internals_.end())
                return it->second;

			internals_[resource_id] = create(cache_.get(resource_id));
            return internals_[resource_id];
        }

        // TODO 'release' internal resources if the public resource's refcount goes to zero.

        virtual std::shared_ptr<InteralType> create(const PublicType& public_resource) = 0;

    private:
        resource_manager(const resource_manager<PublicType, InteralType>&) = delete;
        resource_manager<PublicType, InteralType>& operator=(const resource_manager<PublicType, InteralType>&) = delete;

        resource_cache<PublicType>& cache_;
        std::unordered_map<resource::identifier, std::shared_ptr<InteralType>> internals_;
    };
}
}

#endif // SIGMA_ENGINE_RESOURCE_MANAGER_HPP

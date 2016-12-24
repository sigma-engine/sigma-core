#include <sigma/graphics/static_mesh.hpp>

namespace sigma {
namespace graphics {
	static_mesh_cache::static_mesh_cache(boost::filesystem::path cache_directory, material_cache &materials)
		: resource::resource_cache<static_mesh>(cache_directory)
		, materials_(materials)
	{
	}

	bool static_mesh_cache::increment_reference(resource::identifier resource_id)
	{
		auto r = resource::resource_cache<static_mesh>::increment_reference(resource_id);
		if (r) {
			auto &sm = resources_[resource_id];
			// TODO what if sub resources are missing???
			materials_.increment_reference(sm.material);
		}
		return r;
	}

	bool static_mesh_cache::decrement_reference(resource::identifier resource_id)
	{
		auto r = resource::resource_cache<static_mesh>::decrement_reference(resource_id);
		if (is_cached(resource_id)) {
			auto &sm = resources_[resource_id];
			materials_.decrement_reference(sm.material);
		}
		return r;
	}
}
}

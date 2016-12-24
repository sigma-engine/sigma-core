#include <sigma/graphics/material.hpp>

namespace sigma {
namespace graphics {
	material_cache::material_cache(boost::filesystem::path cache_directory, texture_cache &textures, shader_cache &shaders)
		: resource::resource_cache<material>(cache_directory)
		, textures_(textures)
		, shaders_(shaders)
	{
	}

	bool material_cache::increment_reference(resource::identifier resource_id)
	{
		auto r = resource::resource_cache<material>::increment_reference(resource_id);
		if (r) {
			auto &new_material = resources_[resource_id];
			// TODO what if sub resources are missing???
			shaders_.increment_reference(new_material.vertex_shader);
			shaders_.increment_reference(new_material.fragment_shader);
			for (const auto& texture : new_material.textures)
				textures_.increment_reference(texture.second);
		}
		return r;
	}

	bool material_cache::decrement_reference(resource::identifier resource_id) 
	{
		auto r = resource::resource_cache<material>::decrement_reference(resource_id);
		if (is_cached(resource_id)) {
			auto &mat = resources_[resource_id];
			shaders_.decrement_reference(mat.vertex_shader);
			shaders_.decrement_reference(mat.fragment_shader);
			for (const auto& texture : mat.textures)
				textures_.decrement_reference(texture.second);
		}
		return r;
	}
}
}

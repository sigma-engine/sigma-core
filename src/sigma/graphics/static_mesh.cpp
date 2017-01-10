#include <sigma/graphics/static_mesh.hpp>

namespace sigma {
namespace graphics {

	static_mesh::static_mesh(const static_mesh_data& data)
	{
		for (const auto &mat_slot : data.materials) {
			materials_.push_back(mat_slot.first);
			material_slots_.push_back(mat_slot.second);
		}
	}

	std::size_t static_mesh::material_count() const
	{
		return materials_.size();
	}

    resource::handle<graphics::material>& static_mesh::material(std::size_t slot)
    {
        return materials_[slot];
    }

    void static_mesh::set_material(std::size_t slot, resource::handle<graphics::material> mat)
    {
		// TODO maybe if the material handle passed in is not valid
		// and one of the other materials have is valid 
		// copy the manager into the invalid handle.
        if (materials_[slot] != mat)
            materials_[slot] = mat;
    }
}
}

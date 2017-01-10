#include <sigma/opengl/material.hpp>

#include <sigma/opengl/geometry_buffer.hpp>
#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    material::material(const graphics::material_data& data)
        : shader_technique<graphics::material>(data)
    {
    }

    void material::link()
    {
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::NORMAL_METALNESS_LOCATION, geometry_buffer::NORMAL_METALNESS_OUTPUT_NAME));
        shader_technique::link();
    }

    material_manager::material_manager(boost::filesystem::path cache_directory, opengl::texture_manager& textures, opengl::shader_manager& shaders)
        : graphics::material_manager(cache_directory)
        , textures_(textures)
        , shaders_(shaders)
    {
    }

    std::unique_ptr<graphics::material> material_manager::load(graphics::material_data data, boost::archive::binary_iarchive& ia)
    {
        auto mat = std::make_unique<opengl::material>(data);

		// TODO add other shader types here
		for (auto type : { graphics::shader_type::vertex,graphics::shader_type::fragment,graphics::shader_type::geometry }) {
			if(mat->has_shader(type))
				mat->shader(type).set_manager(&shaders_);
		}
        mat->link();
        for (unsigned int i = 0; i < mat->texture_count(); ++i)
            mat->texture(i).set_manager(&textures_);
        // TODO cubemap
        return std::move(mat);
    }
}
}

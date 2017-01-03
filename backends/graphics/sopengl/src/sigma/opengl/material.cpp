#include <sigma/opengl/material.hpp>

#include <sigma/opengl/geometry_buffer.hpp>
#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {

    void material::link()
    {
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_LOCATION, geometry_buffer::DIFFUSE_ROUGHNESS_OUTPUT_NAME));
        GL_CHECK(glBindFragDataLocation(object_, geometry_buffer::NORMAL_METALNESS_LOCATION, geometry_buffer::NORMAL_METALNESS_OUTPUT_NAME));
        shader_technique::link();
    }

    material_manager::material_manager(boost::filesystem::path cache_directory, opengl::texture_manager &textures, opengl::shader_manager &shaders)
        : graphics::material_manager(cache_directory)
        , textures_(textures)
        , shaders_(shaders)
    {
    }

    std::unique_ptr<graphics::material> material_manager::load(graphics::material_data data, boost::archive::binary_iarchive &ia)
    {
        auto mat = std::make_unique<opengl::material>();
        for(const auto &shdr: data.shaders)
            mat->attach(shaders_.get(shdr.second));
        mat->link();
        for(const auto &txt: data.textures)
            mat->set_texture(txt.first,textures_.get(txt.second));
        return std::move(mat);
    }
}
}

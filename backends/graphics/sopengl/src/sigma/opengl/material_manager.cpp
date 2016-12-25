#include <sigma/opengl/material_manager.hpp>

#include <sigma/opengl/texture_manager.hpp>
#include <sigma/opengl/shader_manager.hpp>

namespace sigma {
namespace opengl {
    material_manager::material_manager(resource::resource_cache<graphics::material>& cache, texture_manager& textures, shader_manager& shaders)
        : resource::resource_manager<graphics::material, opengl::material>(cache)
        , textures_(textures)
        , shaders_(shaders)
    {
    }

    std::shared_ptr<opengl::material> material_manager::create(const graphics::material& cpu_material)
    {
        auto mat = std::make_shared<opengl::material>();
        mat->attach(shaders_.get(cpu_material.vertex_shader));
        mat->attach(shaders_.get(cpu_material.fragment_shader));
        mat->link();

        for (auto texture : cpu_material.textures)
            mat->set_texture(texture.first, textures_.get(texture.second));

        return mat;
    }
}
}

#ifndef SIGMA_ENGINE_MATERIAL_HPP
#define SIGMA_ENGINE_MATERIAL_HPP

#include <sigma/graphics/material.hpp>
#include <sigma/graphics/opengl/shader_technique.hpp>

// #define MATERIAL_PTR(material_mgr, x) static_cast<const sigma::opengl::material*>(material_mgr.acquire(x))
#define MATERIAL_PTR(material_mgr, x) static_cast<sigma::opengl::material*>(material_mgr.acquire(x))

namespace sigma {
namespace opengl {
    class material : public shader_technique<graphics::material> {
    public:
        material() = default;

        material(texture_manager& textures, cubemap_manager& cubemaps, shader_manager& shaders, const graphics::material_data& data);

        material(material&&) = default;

        material& operator=(material&&) = default;

        void link(opengl::shader_manager& shader_mgr);

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;
    };

    class material_manager : public sigma::graphics::material_manager {
    public:
        material_manager(boost::filesystem::path cache_directory, opengl::shader_manager& shaders, opengl::texture_manager& textures, opengl::cubemap_manager& cubemaps);

        virtual std::unique_ptr<graphics::material> create(graphics::material_data data) override;

    private:
        opengl::shader_manager& shaders_;
        opengl::texture_manager& textures_;
        opengl::cubemap_manager& cubemaps_;
    };
}
}

#endif // SIGMA_ENGINE_MATERIAL_HPP

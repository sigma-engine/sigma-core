#ifndef SIGMA_ENGINE_MATERIAL_HPP
#define SIGMA_ENGINE_MATERIAL_HPP

#include <sigma/graphics/material.hpp>
#include <sigma/opengl/shader_technique.hpp>

#include <sigma/opengl/shader.hpp>
#include <sigma/opengl/texture.hpp>

#define MATERIAL_CONST_PTR(x) static_cast<const sigma::opengl::material*>(x.get())
#define MATERIAL_PTR(x) static_cast<sigma::opengl::material*>(x.get())

namespace sigma {
namespace opengl {
    class material : public graphics::material, public shader_technique {
    public:
        material() = default;

        material(material&&) = default;

        material& operator=(material&&) = default;

        void link();

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;
    };

    class material_manager : public graphics::material_manager {
    public:
        material_manager(boost::filesystem::path cache_directory, opengl::texture_manager& textures, opengl::shader_manager& shaders);

        virtual std::unique_ptr<graphics::material> load(graphics::material_data data, boost::archive::binary_iarchive& ia) override;

    private:
        opengl::texture_manager& textures_;
        opengl::shader_manager& shaders_;
    };
}
}

#endif // SIGMA_ENGINE_MATERIAL_HPP

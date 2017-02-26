#ifndef SIGMA_ENGINE_MATERIAL_HPP
#define SIGMA_ENGINE_MATERIAL_HPP

#include <sigma/graphics/material.hpp>
#include <sigma/graphics/opengl/shader_technique.hpp>

#define MATERIAL_CONST_PTR(x) static_cast<const sigma::opengl::material*>(x.get())
#define MATERIAL_PTR(x) static_cast<sigma::opengl::material*>(x.get())

namespace sigma {
namespace opengl {
    class material : public shader_technique<graphics::material> {
    public:
        material() = default;

        material(const graphics::material_data& data);

        material(material&&) = default;

        material& operator=(material&&) = default;

        void link();

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;
    };

    using material_manager = shader_technique_manager<material, graphics::material_manager>;
}
}

#endif // SIGMA_ENGINE_MATERIAL_HPP

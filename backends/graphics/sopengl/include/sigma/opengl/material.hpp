#ifndef SIGMA_ENGINE_MATERIAL_HPP
#define SIGMA_ENGINE_MATERIAL_HPP

#include <sigma/opengl/shader_technique.hpp>

namespace sigma {
namespace opengl {
    class material : public shader_technique {
    public:
        material() = default;

        material(material&&) = default;

        material& operator=(material&&) = default;

        void link();

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;
    };
}
}

#endif // SIGMA_ENGINE_MATERIAL_HPP

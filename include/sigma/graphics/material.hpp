#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/graphics/shader_technique.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    class SIGMA_API material : public shader_technique {
    public:
        material() = default;

        material(material&&) = default;

        material& operator=(material&&) = default;

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;
    };

    using material_cache = shader_technique_cache<material>;
}
}

#endif // SIGMA_GRAPHICS_MATERIAL_HPP

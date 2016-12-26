#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/graphics/shader_technique.hpp>

namespace sigma {
namespace graphics {

    RCLASS()
    class SIGMA_API material : public shader_technique {
    public:
    };

	using material_cache = shader_technique_cache<material>;
}
}

#endif // SIGMA_GRAPHICS_MATERIAL_HPP

#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/graphics/shader_technique.hpp>

#include <boost/serialization/version.hpp>

namespace sigma {
namespace graphics {
    struct material : public shader_technique {
        bool transparent = false;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& boost::serialization::base_object<shader_technique>(*this);
            ar& transparent;
        }
    };
}
}

BOOST_CLASS_VERSION(sigma::graphics::material, 1);

#endif // SIGMA_GRAPHICS_MATERIAL_HPP

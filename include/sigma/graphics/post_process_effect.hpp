#ifndef SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
#define SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

#include <sigma/graphics/shader_technique.hpp>

#include <boost/serialization/version.hpp>

namespace sigma {
namespace graphics {
    struct post_process_effect : public shader_technique {
        boost::filesystem::path mesh = "static_mesh/fullscreen_quad";

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& boost::serialization::base_object<shader_technique>(*this);
            ar& mesh;
        }
    };
}
}

BOOST_CLASS_VERSION(sigma::graphics::post_process_effect, 1);

#endif //SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

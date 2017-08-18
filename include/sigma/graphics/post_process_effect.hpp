#ifndef SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
#define SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/technique.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/version.hpp>

namespace sigma {
namespace graphics {
    struct post_process_effect : public technique_uniform_data {
        technique_identifier technique_id;
        boost::filesystem::path mesh = "static_mesh/fullscreen_quad";

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& boost::serialization::base_object<technique_uniform_data>(*this);
            ar& technique_id;
            ar& mesh;
        }
    };
}
}

BOOST_CLASS_VERSION(sigma::graphics::post_process_effect, 1);

#endif //SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

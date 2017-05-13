#ifndef SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP
#define SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

#include <sigma/config.hpp>

#include <sigma/graphics/shader_technique.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/resource/identifier.hpp>
#include <sigma/resource/manager.hpp>

#include <boost/serialization/version.hpp>

namespace sigma {
namespace graphics {
    struct post_process_effect_data : public shader_technique_data {
        resource::identifier mesh = "static_mesh://fullscreen_quad:plane";

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& boost::serialization::base_object<shader_technique_data>(*this);
            ar& mesh;
        }
    };

    class SIGMA_API post_process_effect : public shader_technique {
    public:
        using resource_data = post_process_effect_data;

        post_process_effect() = default;

        post_process_effect(const post_process_effect_data& data);

        post_process_effect(post_process_effect&&) = default;

        post_process_effect& operator=(post_process_effect&&) = default;

        virtual ~post_process_effect() = default;

        resource::handle<static_mesh>& mesh();

        const resource::handle<static_mesh>& mesh() const;

        void set_mesh(resource::handle<static_mesh> mesh);

    protected:
        resource::handle<static_mesh> mesh_;

    private:
        post_process_effect(const post_process_effect&) = delete;
        post_process_effect& operator=(const post_process_effect&) = delete;
    };

    using post_process_effect_manager = resource::manager<post_process_effect>;
}
}

BOOST_CLASS_VERSION(sigma::graphics::post_process_effect_data, 1);

#endif //SIGMA_ENGINE_GRAPHICS_POST_PROCESS_EFFECT_HPP

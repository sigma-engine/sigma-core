#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/config.hpp>
#include <sigma/graphics/shader_technique.hpp>
#include <sigma/resource/manager.hpp>

#include <boost/serialization/version.hpp>

namespace sigma {
namespace graphics {
    struct material_data : public shader_technique_data {
        bool transparent = false;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& boost::serialization::base_object<shader_technique_data>(*this);
            ar& transparent;
        }
    };

    class SIGMA_API material : public shader_technique {
    public:
        using resource_data = material_data;

        material() = default;

        material(const material_data& data);

        material(material&&) = default;

        material& operator=(material&&) = default;

        virtual ~material() = default;

        bool is_transparent() const;

    protected:
        bool transparent_;

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;
    };

    using material_manager = resource::manager<material>;
}
}

BOOST_CLASS_VERSION(sigma::graphics::material_data, 1);

#endif // SIGMA_GRAPHICS_MATERIAL_HPP

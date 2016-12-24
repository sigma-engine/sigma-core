#ifndef SIGMA_GRAPHICS_MATERIAL_HPP
#define SIGMA_GRAPHICS_MATERIAL_HPP

#include <sigma/config.hpp>

#include <boost/serialization/access.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>

#include <sigma/reflect/reflect.hpp>
#include <sigma/resource/identifier.hpp>
#include <sigma/resource/resource_cache.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/graphics/shader.hpp>

namespace sigma {
namespace graphics {
	
    RCLASS()
    class SIGMA_API material {
    public:
        material() = default;

        material(const material&) = delete;

        material(material&&) = default;

        material& operator=(const material&) = delete;

        material& operator=(material&&) = default;

        ~material() = default;

        resource::identifier vertex_shader;
        resource::identifier fragment_shader;
        std::vector<std::pair<std::string, resource::identifier> > textures;

        template <class Archive>
        void serialize(Archive& ar, const unsigned int version)
        {
            ar& vertex_shader;
            ar& fragment_shader;
            ar& textures;
        }

		friend class material_cache;
		std::size_t reference_count = 0;
    };

    class SIGMA_API material_cache : public resource::resource_cache<material> {
    public:
		material_cache(boost::filesystem::path cache_directory, texture_cache &textures, shader_cache &shaders);

		virtual bool increment_reference(resource::identifier resource_id) override;

		virtual bool decrement_reference(resource::identifier resource_id) override;
	private:
		texture_cache &textures_;
		shader_cache &shaders_;
    };
}
}

#endif // SIGMA_GRAPHICS_MATERIAL_HPP

#ifndef SIGMA_FIVE_OPENGL_TEXTURE_HPP
#define SIGMA_FIVE_OPENGL_TEXTURE_HPP

#include <sigmafive/opengl/config.hpp>

#include <sigmafive/graphics/texture.hpp>

#include <chrono>
#include <unordered_map>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <glm/mat4x4.hpp>

#include <sigmafive/opengl/gl_core_4_0.h>
#include <sigmafive/opengl/shader.hpp>

namespace sigmafive {
namespace opengl {

    /*struct texture_v2 : public graphics::texture {
    GLuint object = 0;
    texture_v2() = default;

    ~texture_v2() {
        if(object != 0)
            glDeleteTextures(1,&object);
    }
};*/

    struct texture : public graphics::texture {
        texture() = default;

        texture(const texture&) = delete;

        texture(texture&&) = default;

        texture& operator=(const texture&) = delete;

        texture& operator=(texture&&) = default;

        ~texture() = default;

        GLuint object = 0;
        std::size_t reference_count = 0;
    };

    class SIGMA_FIVE_OPENGL_API texture_cache : public graphics::texture_cache {
    public:
        texture_cache();

        texture_cache(const texture_cache&) = delete;

        texture_cache(texture_cache&&) = default;

        texture_cache& operator=(const texture_cache&) = delete;

        texture_cache& operator=(texture_cache&&) = default;

        virtual ~texture_cache();

        virtual bool is_cached(resource::identifier texture) const override;

        virtual bool increment_reference(resource::identifier texture) override;

        virtual bool decrement_reference(resource::identifier texture) override;

        void bind(resource::identifier texture_id) const;

        void update();

    private:
        using size_type = std::vector<texture>::size_type;
        using difference_type = std::vector<texture>::difference_type;

        boost::filesystem::path cache_directory_;
        std::unordered_map<resource::constexpr_identifier, size_type> resource_map_;

        std::vector<texture> textures_;
        size_type dirty_;
    };
}
}

#endif // SIGMA_FIVE_OPENGL_TEXTURE_HPP

#ifndef SIGMA_FIVE_OPENGL_SHADER_HPP
#define SIGMA_FIVE_OPENGL_SHADER_HPP

#include <sigma/opengl/config.hpp>

#include <unordered_map>
#include <vector>

#include <sigma/graphics/shader.hpp>

#include <boost/filesystem/path.hpp>

#include <sigma/opengl/gl_core_4_0.h>

namespace sigma {
namespace opengl {

    struct shader : public graphics::shader {
        GLuint object = 0;
        std::size_t reference_count = 0;
    };

    class SIGMA_FIVE_OPENGL_API shader_cache : public graphics::shader_cache {
    public:
        shader_cache();

        ~shader_cache();

        shader_cache(const shader_cache&) = delete;

        shader_cache& operator=(const shader_cache&) = delete;

        virtual bool is_cached(resource::identifier shader) const override;

        virtual bool increment_reference(resource::identifier shader) override;

        virtual bool decrement_reference(resource::identifier shader) override;

        GLuint get_object(resource::identifier shader);

        void update();

    private:
        using size_type = std::vector<shader>::size_type;
        using difference_type = std::vector<shader>::difference_type;

        boost::filesystem::path cache_directory_;
        std::unordered_map<resource::identifier, size_type> resource_map_;

        std::vector<shader> shaders_;
        size_type dirty_;
    };
}
}

#endif // SIGMA_FIVE_OPENGL_SHADER_HPP

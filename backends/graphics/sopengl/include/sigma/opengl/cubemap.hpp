#ifndef SIGMA_ENGINE_OPENGL_CUBEMAP_HPP
#define SIGMA_ENGINE_OPENGL_CUBEMAP_HPP

#include <sigma/graphics/cubemap.hpp>

#include <sigma/opengl/gl_core_4_2.h>

#include <vector>

#define CUBEMAP_CONST_PTR(x) static_cast<const sigma::opengl::cubemap*>(x.get())
#define CUBEMAP_PTR(x) static_cast<sigma::opengl::cubemap*>(x.get())

namespace sigma {
namespace opengl {

    class cubemap : public graphics::cubemap {
    public:
        cubemap(graphics::cubemap_data data);

        cubemap(cubemap&&) = default;

        cubemap& operator=(cubemap&&) = default;

        ~cubemap();

        GLuint get_object() const;

        void bind();

    private:
        cubemap(const cubemap&) = delete;
        cubemap& operator=(const cubemap&) = delete;

        GLuint object_ = 0;
    };

    class cubemap_manager : public graphics::cubemap_manager {
    public:
        using graphics::cubemap_manager::cubemap_manager;

        virtual std::unique_ptr<graphics::cubemap> load(graphics::cubemap_data data, boost::archive::binary_iarchive& ia) override;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_CUBEMAP_HPP

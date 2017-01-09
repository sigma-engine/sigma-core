#ifndef SIGMA_ENGINE_STATIC_MESH_HPP
#define SIGMA_ENGINE_STATIC_MESH_HPP

#include <sigma/graphics/static_mesh.hpp>

#include <sigma/opengl/gl_core_4_2.h>
#include <sigma/opengl/material.hpp>

#define STATIC_MESH_CONST_PTR(x) static_cast<const sigma::opengl::static_mesh*>(x.get())
#define STATIC_MESH_PTR(x) static_cast<sigma::opengl::static_mesh*>(x.get())

namespace sigma {
namespace opengl {
    struct instance_matrices;
    class shader_technique;
    enum class texture_unit : GLenum;
    class static_mesh : public graphics::static_mesh {
    public:
        static_mesh(graphics::static_mesh_data data);

        static_mesh(static_mesh&&) = default;

        static_mesh& operator=(static_mesh&&) = default;

        ~static_mesh();

        void render();

        // TODO make private
        GLuint vertex_array_ = 0;
        GLuint vertex_buffer_ = 0;
        GLuint index_buffer_ = 0;
        GLuint index_count_ = 0;

    private:
        static_mesh(const static_mesh&) = delete;
        static_mesh& operator=(const static_mesh&) = delete;
    };

    class static_mesh_manager : public graphics::static_mesh_manager {
    public:
        static_mesh_manager(boost::filesystem::path cache_directory, opengl::material_manager& materials);

        virtual std::unique_ptr<graphics::static_mesh> load(graphics::static_mesh_data data, boost::archive::binary_iarchive& ia) override;

    private:
        opengl::material_manager& materials_;
    };
}
}

#endif // SIGMA_ENGINE_STATIC_MESH_HPP

#ifndef SIGMA_ENGINE_OPENGL_STATIC_MESH_MANAGER_HPP
#define SIGMA_ENGINE_OPENGL_STATIC_MESH_MANAGER_HPP

#include <sigma/opengl/static_mesh.hpp>

#include <sigma/graphics/static_mesh.hpp>
#include <sigma/resource/resource_manager.hpp>

namespace sigma {
namespace opengl {
    class material_manager;
    class static_mesh_manager : public resource::resource_manager<graphics::static_mesh, opengl::static_mesh> {
    public:
        static_mesh_manager(resource::resource_cache<graphics::static_mesh>& cache, material_manager& materials);

        virtual std::shared_ptr<opengl::static_mesh> create(const graphics::static_mesh& cpu_static_mesh) override;

    private:
        static_mesh_manager(const static_mesh_manager&) = delete;
        static_mesh_manager& operator=(const static_mesh_manager&) = delete;

        material_manager& materials_;
    };
}
}

#endif // SIGMA_ENGINE_OPENGL_STATIC_MESH_MANAGER_HPP

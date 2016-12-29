#include <sigma/opengl/static_mesh_manager.hpp>

#include <sigma/opengl/material_manager.hpp>

namespace sigma {
namespace opengl {
    static_mesh_manager::static_mesh_manager(resource::resource_cache<graphics::static_mesh>& cache, material_manager& materials)
        : resource::resource_manager<graphics::static_mesh, opengl::static_mesh>(cache)
        , materials_(materials)
    {
    }

    std::shared_ptr<opengl::static_mesh> static_mesh_manager::create(const graphics::static_mesh& cpu_static_mesh)
    {
        return std::make_shared<opengl::static_mesh>(cpu_static_mesh.vertices, cpu_static_mesh.triangles, materials_.get(cpu_static_mesh.material));
    }
}
}

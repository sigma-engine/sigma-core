#include <sigma/graphics/opengl/renderer.hpp>

#include <glad/glad.h>
#include <sigma/graphics/opengl/shader.hpp>
#include <sigma/graphics/opengl/texture.hpp>
#include <sigma/graphics/opengl/util.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/vec4.hpp>

namespace sigma {
namespace opengl {
    renderer::renderer(glm::ivec2 size)
        : graphics::renderer(size)
        , loader_status_(gladLoadGL())
        , default_fbo_(size)
        , gbuffer_(size)
        , shaders_(boost::filesystem::current_path() / ".." / "data")
        , textures_(boost::filesystem::current_path() / ".." / "data")
        , cubemaps_(boost::filesystem::current_path() / ".." / "data")
        , materials_(boost::filesystem::current_path() / ".." / "data", shaders_, textures_, cubemaps_)
        , static_meshes_(boost::filesystem::current_path() / ".." / "data", materials_)
        , effects_(boost::filesystem::current_path() / ".." / "data", shaders_, textures_, cubemaps_, static_meshes_)
    {
        if (!loader_status_)
            throw std::runtime_error("error: could not load OpenGL");

        //standard_uniforms_.set_binding_point(shader_technique::STANDARD_UNIFORM_BLOCK_BINDING);

        image_based_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/image_based");
        point_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/point");
        directional_light_effect_ = effects_.get("post_process_effect://pbr/deffered/lights/directional");
        texture_blit_effect_ = effects_.get("post_process_effect://pbr/deffered/texture_blit");

        vignette_effect_ = effects_.get("post_process_effect://vignette");
        gamma_conversion_ = effects_.get("post_process_effect://pbr/deffered/gamma_conversion");
    }

    renderer::~renderer()
    {
    }

    graphics::texture_manager& renderer::textures()
    {
        return textures_;
    }

    graphics::cubemap_manager& renderer::cubemaps()
    {
        return cubemaps_;
    }

    graphics::shader_manager& renderer::shaders()
    {
        return shaders_;
    }

    graphics::material_manager& renderer::materials()
    {
        return materials_;
    }

    graphics::static_mesh_manager& renderer::static_meshes()
    {
        return static_meshes_;
    }

    graphics::post_process_effect_manager& renderer::effects()
    {
        return effects_;
    }

    void renderer::resize(glm::uvec2 size)
    {
        GL_CHECK(glViewport(0, 0, size.x, size.y));
    }

    // void renderer::render_material_buckets(std::vector<material_bucket>& material_buckets)
    // {
    //     for (const auto& mat_bucket : material_buckets) { // TODO const
    //         auto mat = MATERIAL_CONST_PTR(mat_bucket.active_material);
    //         mat->bind(geometry_buffer::NEXT_FREE_TEXTURE_UINT);
    //         mat->set_standard_uniforms(&standard_uniform_data_);
    //         for (const auto& mesh_bucket : mat_bucket.mesh_buckets) { // TODO const
    //             auto mesh = STATIC_MESH_CONST_PTR(mesh_bucket.active_mesh);
    //             auto material_slot = mesh_bucket.material_slot;
    //
    //             instance_matrices matrices;
    //             for (const auto& instance : mesh_bucket.instances) { // TODO const
    //                 matrices.model_matrix = instance.model_matrix;
    //                 matrices.model_view_matrix = standard_uniform_data_.view_matrix * instance.model_matrix;
    //                 matrices.normal_matrix = glm::transpose(glm::inverse(glm::mat3(matrices.model_view_matrix)));
    //                 mat->set_instance_matrices(&matrices);
    //                 mesh->render(material_slot);
    //             }
    //         }
    //     }
    // }

    // std::size_t renderer::fill_material_buckets(const graphics::view_port& viewport, std::vector<material_bucket>& material_buckets, bool transparent)
    // {
    //     std::size_t max_instances = 0;
    //     for (auto e : viewport.entities) { // TODO use a filter here
    //         if (viewport.static_mesh_instances.has(e) && viewport.transforms.has(e)) {
    //             auto& txform = viewport.transforms.get(e);
    //             auto mesh = viewport.static_mesh_instances.get(e);
    //             for (unsigned int i = 0; i < mesh->material_count(); ++i) {
    //                 auto mat = mesh->material(i);
    //
    //                 if (mat->is_transparent() == transparent) {
    //                     // find the material bucket if it exist
    //                     auto mat_it = std::find_if(material_buckets.begin(), material_buckets.end(), [&](const auto& bucket) {
    //                         return bucket.active_material == mat;
    //                     });
    //                     // create the material bucket if it does not exist
    //                     if (mat_it == material_buckets.end())
    //                         mat_it = material_buckets.insert(mat_it, { mat, {} });
    //
    //                     // find the mesh bucket if it exist
    //                     auto mesh_it = std::find_if(mat_it->mesh_buckets.begin(), mat_it->mesh_buckets.end(), [&](const auto& bucket) {
    //                         return bucket.active_mesh == mesh; // TODO do we need to match the material index too??
    //                     });
    //
    //                     if (mesh_it == mat_it->mesh_buckets.end())
    //                         mesh_it = mat_it->mesh_buckets.insert(mesh_it, { mesh, i, {} });
    //
    //                     mesh_it->instances.push_back({ txform.matrix() });
    //
    //                     max_instances = std::max(max_instances, mesh_it->instances.size());
    //                 }
    //             }
    //         }
    //     }
    //     return max_instances;
    // }

    /*void renderer::point_light_outside_stencil_optimization(glm::vec3 view_space_position, float radius)
	{
		// http://forum.devmaster.net/t/deferred-lighting-rendering-light-volumes/14998/5
		if (glm::length(view_space_position) > 1.1 * radius) {
			gbuffer_.bind_for_stencil_pass();

			GL_CHECK(glEnable(GL_STENCIL_TEST));
			GL_CHECK(glClearStencil(4));
			GL_CHECK(glClear(GL_STENCIL_BUFFER_BIT));

			GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE));
			GL_CHECK(glStencilFunc(GL_GEQUAL, 6, 0xFF));

			GL_CHECK(glDepthFunc(GL_GEQUAL));
			GL_CHECK(glCullFace(GL_FRONT));

			EFFECT_PTR(point_light_stencil_effect_)->apply(&matrices_);

			GL_CHECK(glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP));
			GL_CHECK(glStencilFunc(GL_LEQUAL, 5, 0xFF));

			GL_CHECK(glDepthFunc(GL_LEQUAL));
			GL_CHECK(glCullFace(GL_BACK));
		}
	}*/
}
}

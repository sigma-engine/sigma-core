#ifndef SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

#include <sigmafive/engine.hpp>

#include <shader.hpp>
#include <program.hpp>
#include <material.hpp>
#include <static_mesh.hpp>
#include <static_mesh_manager.hpp>

#include <sigmafive/factory.hpp>
#include <sigmafive/graphics/context.hpp>
#include <sigmafive/system/resource_manager.hpp>

#include <queue>
#include <memory>

extern "C" {
    void __declspec(dllexport) register_plugin(sigmafive::engine *engine);
}

namespace sigmafive {
	namespace graphics {
		namespace opengl {
            struct static_mesh_instance {
                float4x4 model_matrix;
                boost::uuids::uuid static_mesh;
            };

            class context : public graphics::context {
                SIGMAFIVE_CLASS()
			public:
                context(system::resource_manager &resource_manager);

                virtual void make_current() override;

                virtual void add_static_mesh(float4x4 model_matrix,boost::uuids::uuid static_mesh) override;

                virtual void render(float4x4 projection_matrix,float4x4 view_matrix) override;

                virtual void swap_buffers() override;
			private:
                std::queue<static_mesh_instance> static_meshes_;
                opengl::material material_;

				opengl::shader vertex_shader;
				opengl::shader fragment_shader;

                system::resource_manager &resource_manager_;
                opengl::static_mesh_manager static_mesh_manager_;
            };

            class context_factory : public factory<graphics::context> {
            public:
                context_factory(system::resource_manager &resource_manager);

                virtual std::unique_ptr<graphics::context> create() override;
            private:
                system::resource_manager &resource_manager_;
            };
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

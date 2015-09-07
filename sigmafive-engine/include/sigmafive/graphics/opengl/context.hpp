#ifndef SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_SCENE_RENDERER_HPP

#include <sigmafive/graphics/context.hpp>
#include <sigmafive/system/resource_manager.hpp>

#include <sigmafive/graphics/opengl/shader.hpp>
#include <sigmafive/graphics/opengl/program.hpp>
#include <sigmafive/graphics/opengl/material.hpp>
#include <sigmafive/graphics/opengl/static_mesh.hpp>
#include <sigmafive/graphics/opengl/static_mesh_manager.hpp>

#include <memory>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
            class context : public graphics::context {
                SIGMAFIVE_CLASS()
			public:
                context(system::resource_manager &resource_manager);

                virtual void make_current() override;

                virtual void render(float4x4 projection_matrix,float4x4 view_matrix,const game::scene &scene) override;

                virtual void swap_buffers() override;
			private:
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

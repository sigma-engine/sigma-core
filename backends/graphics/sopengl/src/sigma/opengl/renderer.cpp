#include <sigma/opengl/renderer.hpp>

#include <boost/filesystem.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sigma/opengl/gl_core_4_0.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <glm/mat4x4.hpp>

namespace sigma {
namespace opengl {
    const resource::identifier renderer::PLANE_STATIC_MESH{ "static_mesh://plane:Plane" };
    const resource::identifier renderer::FULLSCREEN_MATERIAL1{ "material://fullscreen_quad" };
    const resource::identifier renderer::FULLSCREEN_MATERIAL2{ "material://fullscreen_quad2" };

    renderer::renderer(context *ctx)
        : graphics::renderer(ctx)
        , ctx_(ctx)
		, texture_cache_(ctx->textures())
		, shader_cache_(ctx->shaders())
		, material_cache_(ctx->materials())
		, static_mesh_cache_(ctx->static_meshes())
    {
        material_cache_.increment_reference(FULLSCREEN_MATERIAL1);
        material_cache_.increment_reference(FULLSCREEN_MATERIAL2);
        static_mesh_cache_.increment_reference(PLANE_STATIC_MESH);
        std::cout << glGetString(GL_VERSION) << std::endl;;
    }

    renderer::~renderer()
    {
        static_mesh_cache_.decrement_reference(PLANE_STATIC_MESH);
        material_cache_.decrement_reference(FULLSCREEN_MATERIAL2);
        material_cache_.decrement_reference(FULLSCREEN_MATERIAL1);

		for (const auto &mesh : static_meshes_) {
			glDeleteVertexArrays(1, &mesh.second.vertex_array);
			glDeleteBuffers(1, &mesh.second.vertex_buffer);
			glDeleteBuffers(1, &mesh.second.index_buffer);
		}
		
		for (const auto &mat : materials_) {
			glDeleteProgram(mat.second.object);
		}
		
		for (const auto &shd : shaders_) {
			glDeleteShader(shd.second.object);
		}

		for (const auto &txt : textures_) {
			glDeleteTextures(1, &txt.second.object);
		}
    }

    void renderer::resize(glm::uvec2 size)
    {
        // TODO resize
        glViewport(0,0,size.x,size.y);
    }

    opengl::texture renderer::get_texture(resource::identifier id)
    {
        if(textures_.count(id) > 0)
            return textures_[id];

        if(texture_cache_.is_loaded(id)) {
            auto &rtexture = texture_cache_.get(id);

            opengl::texture txt;
            GL_CHECK(glGenTextures(1, &txt.object));
            GL_CHECK(glBindTexture(GL_TEXTURE_2D, txt.object));
            GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
            GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
            GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, rtexture.width(), rtexture.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, rtexture.data().data()));

            textures_[id] = txt;
            return textures_[id];
        }
        return opengl::texture{};
    }

	GLenum convert(graphics::shader_type type)
	{
		switch (type) {
		case graphics::shader_type::vertex:
			return GL_VERTEX_SHADER;
		case graphics::shader_type::fragment:
			return GL_FRAGMENT_SHADER;
		case graphics::shader_type::none:
			return GL_NONE;
		}
		return GL_NONE;
	}

    opengl::shader renderer::get_shader(resource::identifier id)
    {
        if(shaders_.count(id) > 0)
            return shaders_[id];

        if(shader_cache_.is_loaded(id)) {
            auto &rshader = shader_cache_.get(id);
            const char* source = rshader.source.c_str();

            opengl::shader shd;
            GL_CHECK(shd.object = glCreateShader(convert(rshader.type)));
            GL_CHECK(glShaderSource(shd.object, 1, &source, nullptr));
            GL_CHECK(glCompileShader(shd.object));

            shaders_[id] = shd;
            return shaders_[id];
        }

        return opengl::shader{};
    }

    opengl::material renderer::get_material(resource::identifier id)
    {
        if(materials_.count(id) > 0)
            return materials_[id];

        if(material_cache_.is_loaded(id)) {
            auto &rmaterial = material_cache_.get(id);

            opengl::material mat;
            GL_CHECK(mat.object = glCreateProgram());
            GL_CHECK(glAttachShader(mat.object, get_shader(rmaterial.vertex_shader).object));
            GL_CHECK(glAttachShader(mat.object,get_shader(rmaterial.fragment_shader).object));
            GL_CHECK(glLinkProgram(mat.object));

            auto texture_count = rmaterial.textures.size();
			mat.texture_locations.resize(texture_count);
            for (int i = 0; i < texture_count; ++i) {
                GL_CHECK(mat.texture_locations[i] = glGetUniformLocation(mat.object, rmaterial.textures[i].first.c_str()));
            }

            GL_CHECK(mat.projection_matrix_location = glGetUniformLocation(mat.object, PROJECTION_MATRIX_NAME));
            GL_CHECK(mat.view_matrix_location = glGetUniformLocation(mat.object, VIEW_MATRIX_NAME));
            GL_CHECK(mat.model_matrix_location = glGetUniformLocation(mat.object, MODEL_MATRIX_NAME));
            GL_CHECK(mat.model_view_matrix_location = glGetUniformLocation(mat.object, MODEL_VIEW_MATRIX_NAME));
            GL_CHECK(mat.normal_matrix_location = glGetUniformLocation(mat.object, NORMAL_MATRIX_NAME));
            GL_CHECK(mat.time_location = glGetUniformLocation(mat.object, TIME_NAME));

            materials_[id] = mat;
            return materials_[id];
        }

        return opengl::material{};
    }

    opengl::static_mesh renderer::get_static_mesh(resource::identifier id)
    {
        if(static_meshes_.count(id) > 0)
            return static_meshes_[id];
		if (static_mesh_cache_.is_loaded(id)) {
			auto &rmesh = static_mesh_cache_.get(id);

			opengl::static_mesh mesh;
			GL_CHECK(glGenVertexArrays(1, &mesh.vertex_array));
			GL_CHECK(glBindVertexArray(mesh.vertex_array));
			GL_CHECK(glGenBuffers(1, &mesh.vertex_buffer));
			GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mesh.vertex_buffer));
			GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(graphics::static_mesh::vertex) * rmesh.vertices.size(), rmesh.vertices.data(), GL_STATIC_DRAW));

			GL_CHECK(glEnableVertexAttribArray(0));
			GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(graphics::static_mesh::vertex), 0));

			GL_CHECK(glEnableVertexAttribArray(1));
			GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, sizeof(graphics::static_mesh::vertex), reinterpret_cast<const void*>(sizeof(glm::vec3))));

			GL_CHECK(glEnableVertexAttribArray(2));
			GL_CHECK(glVertexAttribPointer(2, 3, GL_FLOAT, GL_TRUE, sizeof(graphics::static_mesh::vertex), reinterpret_cast<const void*>(2 * sizeof(glm::vec3))));

			GL_CHECK(glEnableVertexAttribArray(3));
			GL_CHECK(glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(graphics::static_mesh::vertex), reinterpret_cast<const void*>(3 * sizeof(glm::vec3))));

			mesh.index_count = 3 * rmesh.triangles.size();
			GL_CHECK(glGenBuffers(1, &mesh.index_buffer));
			GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer));
			GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh.index_count, rmesh.triangles.data(), GL_STATIC_DRAW));

			mesh.material = rmesh.material;

			static_meshes_[id] = mesh;
			return static_meshes_[id];
		}
        return opengl::static_mesh{};
    }

    void renderer::render(const graphics::view_port& viewport)
    {
        GL_CHECK(glClearColor(.8f, 0.8f, 0.8f, 1.0f));
        GL_CHECK(glEnable(GL_DEPTH_TEST));
        GL_CHECK(glEnable(GL_BLEND));
        GL_CHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GL_CHECK(glCullFace(GL_BACK));
        GL_CHECK(glEnable(GL_CULL_FACE));
        GL_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

        /*textures_.update();
        shaders_.update();
        materials_.update();
        static_meshes_.update();*/

        for (auto e : viewport.entities) { // TODO use a filter here
            if (viewport.static_mesh_instances.has(e) && viewport.transforms.has(e)) {
                const auto& txform = viewport.transforms.get(e);
				const auto &mesh = get_static_mesh(viewport.static_mesh_instances.get(e));
                glm::mat4 model_matrix(1.0f);
                model_matrix = glm::mat4_cast(txform.rotation) * glm::translate(glm::scale(model_matrix, txform.scale), txform.position);

				auto &rmaterial = material_cache_.get(mesh.material);
				const auto &mat = get_material(mesh.material);

				GL_CHECK(glUseProgram(mat.object));
				auto texture_count = rmaterial.textures.size();
				for (int i = 0; i < texture_count; ++i) {
					GL_CHECK(glUniform1i(mat.texture_locations[i], i));
					GL_CHECK(glActiveTexture(GL_TEXTURE0 + i));

					auto &txt = get_texture(rmaterial.textures[i].second);
					GL_CHECK(glBindTexture(GL_TEXTURE_2D, txt.object));
				}

				auto modelViewMatrix = viewport.view_matrix * model_matrix;
				auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelViewMatrix)));
				GL_CHECK(glUniformMatrix4fv(mat.projection_matrix_location, 1, GL_FALSE, glm::value_ptr(viewport.projection_matrix)));
				GL_CHECK(glUniformMatrix4fv(mat.view_matrix_location, 1, GL_FALSE, glm::value_ptr(viewport.view_matrix)));
				GL_CHECK(glUniformMatrix4fv(mat.model_matrix_location, 1, GL_FALSE, glm::value_ptr(model_matrix)));
				GL_CHECK(glUniformMatrix4fv(mat.model_view_matrix_location, 1, GL_FALSE, glm::value_ptr(modelViewMatrix)));
				GL_CHECK(glUniformMatrix3fv(mat.normal_matrix_location, 1, GL_FALSE, glm::value_ptr(normalMatrix)));
				//GL_CHECK(glUniform1f(mat.time_location, elpased_time_.count())); // TODO put elpased time back in
				// TODO fill out the rest of the material uniforms here

				GL_CHECK(glBindVertexArray(mesh.vertex_array));
				GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.index_buffer));
				GL_CHECK(glDrawElements(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, nullptr));
            }
        }

        // static_meshes_.render(PLANE_STATIC_MESH, FULLSCREEN_MATERIAL1,
        // viewport.projection_matrix, viewport.view_matrix, glm::mat4(1.0f));
    }
}
}

SIGMA_EXPORT_RENDERER_CLASSES(sigma::opengl::renderer)

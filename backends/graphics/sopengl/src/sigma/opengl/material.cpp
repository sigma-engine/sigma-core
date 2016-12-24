#include <sigma/opengl/material.hpp>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>
#include <iostream>
#include <sigma/opengl/util.hpp>
#include <string>

namespace sigma {
namespace opengl {
    /*material_cache::~material_cache()
    {
        for (auto& material : materials_)
          glDeleteProgram(material.object);
    }

    void material_cache::apply(resource::identifier material_id,
        const glm::mat4& projectionMatrix,
        const glm::mat4& viewMatrix,
        const glm::mat4& modelMatrix)
    {
        if (!is_cached(material_id))
            return; // TODO apply default instead?
        auto index = resource_map_.at(material_id);

        GL_CHECK(glUseProgram(materials_[index].object))
        auto texture_count = materials_[index].textures.size();
        for (int i = 0; i < texture_count; ++i) {
            GL_CHECK(glUniform1i(materials_[index].texture_locations[i], i));
            GL_CHECK(glActiveTexture(GL_TEXTURE0 + i));
            textures_.bind(materials_[index].textures[i].second);
        }
        auto modelViewMatrix = viewMatrix * modelMatrix;
        auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(modelViewMatrix)));
        GL_CHECK(glUniformMatrix4fv(materials_[index].projection_matrix_location, 1, GL_FALSE, glm::value_ptr(projectionMatrix)));
        GL_CHECK(glUniformMatrix4fv(materials_[index].view_matrix_location, 1, GL_FALSE, glm::value_ptr(viewMatrix)));
        GL_CHECK(glUniformMatrix4fv(materials_[index].model_matrix_location, 1, GL_FALSE, glm::value_ptr(modelMatrix)));
        GL_CHECK(glUniformMatrix4fv(materials_[index].model_view_matrix_location, 1, GL_FALSE, glm::value_ptr(modelViewMatrix)));
        GL_CHECK(glUniformMatrix3fv(materials_[index].normal_matrix_location, 1, GL_FALSE, glm::value_ptr(normalMatrix)));
        GL_CHECK(glUniform1f(materials_[index].time_location, elpased_time_.count()));
        // TODO fill out the rest of the material uniforms here
    }

    void material_cache::update()
    {
        elpased_time_ = std::chrono::duration_cast<std::chrono::duration<float> >(std::chrono::high_resolution_clock::now() - start_time_);
        std::for_each(std::begin(materials_) + static_cast<difference_type>(dirty_), std::end(materials_), [this](opengl::material& material) {
            if (material.object == 0)
                GL_CHECK(material.object = glCreateProgram());

            GL_CHECK(glAttachShader(material.object, shaders_.get_object(material.vertex_shader)));
            GL_CHECK(glAttachShader(material.object, shaders_.get_object(material.fragment_shader)));
            GL_CHECK(glLinkProgram(material.object));

            auto texture_count = material.textures.size();
            material.texture_locations.resize(texture_count);
            for (int i = 0; i < texture_count; ++i) {
                GL_CHECK(material.texture_locations[i] = glGetUniformLocation(material.object, material.textures[i].first.c_str()));
            }

            GL_CHECK(material.projection_matrix_location = glGetUniformLocation(material.object, PROJECTION_MATRIX_NAME));
            GL_CHECK(material.view_matrix_location = glGetUniformLocation(material.object, VIEW_MATRIX_NAME));
            GL_CHECK(material.model_matrix_location = glGetUniformLocation(material.object, MODEL_MATRIX_NAME));
            GL_CHECK(material.model_view_matrix_location = glGetUniformLocation(material.object, MODEL_VIEW_MATRIX_NAME));
            GL_CHECK(material.normal_matrix_location = glGetUniformLocation(material.object, NORMAL_MATRIX_NAME));
            GL_CHECK(material.time_location = glGetUniformLocation(material.object, TIME_NAME));
        });
        dirty_ = materials_.size();
    }*/
}
}

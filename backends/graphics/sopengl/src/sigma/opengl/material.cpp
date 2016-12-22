#include <sigma/opengl/material.hpp>

#include <fstream>
#include <iostream>
#include <string>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/filesystem/operations.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/matrix.hpp>

#include <sigma/opengl/util.hpp>

namespace sigma {
namespace opengl {
    material_cache::material_cache(shader_cache& shaders, texture_cache& textures)
        : cache_directory_(boost::filesystem::current_path() / ".." / "data")
        , dirty_(materials_.size())
        , shaders_(shaders)
        , textures_(textures)
        , start_time_(std::chrono::high_resolution_clock::now())
    {
    }

    material_cache::~material_cache()
    {
        for (auto& material : materials_)
          glDeleteProgram(material.object);
    }

    bool material_cache::is_cached(resource::identifier material_id) const
    {
        return resource_map_.count(material_id) > 0;
    }

    bool material_cache::increment_reference(resource::identifier material_id)
    {
        if (is_cached(material_id)) {
            materials_[resource_map_[material_id]].reference_count++;
            return true;
        }

        material new_material;
        try {
            auto material_path = cache_directory_ / std::to_string(material_id.value());
            std::ifstream file{ material_path.string(), std::ios::binary | std::ios::in };
            boost::archive::binary_iarchive ia{ file };
            ia >> new_material;
        } catch (boost::archive::archive_exception& ex) {
            std::cout << "material: " << material_id << " " << ex.what() << std::endl;
            return false;
        } catch (std::exception& ex) {
            std::cout << "material: " << material_id << " " << ex.what() << std::endl;
            return false;
        } catch (...) { // TODO check for correct errors here
            std::cout << "material: unknown exception " << std::endl;
            return false;
        }

        auto index = materials_.size();
        if (dirty_ >= index)
            dirty_ = index;
        resource_map_[material_id] = index;
        new_material.reference_count = 1;
        shaders_.increment_reference(new_material.vertex_shader);
        shaders_.increment_reference(new_material.fragment_shader);
        for (const auto& texture : new_material.textures)
            textures_.increment_reference(texture.second);
        materials_.push_back(std::move(new_material));

        return true;
    }

    bool material_cache::decrement_reference(resource::identifier material_id)
    {
        if (!is_cached(material_id))
            return false;
        auto index = resource_map_[material_id];
        if (materials_[index].reference_count > 0)
            materials_[index].reference_count--;
        shaders_.decrement_reference(materials_[index].vertex_shader);
        shaders_.decrement_reference(materials_[index].fragment_shader);
        for (const auto& texture : materials_[index].textures)
            textures_.decrement_reference(texture.second);
        return materials_[index].reference_count == 0;
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
    }
}
}

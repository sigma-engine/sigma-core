#ifndef SIGMA_GRAPHICS_FRUSTUM_HPP
#define SIGMA_GRAPHICS_FRUSTUM_HPP

#include <sigma/config.hpp>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include <array>

namespace sigma {
class frustum {
public:
    frustum();

    frustum(float fovy, float aspect, float z_near, float z_far, const glm::mat4& view = {});

    float fovy() const;

    float aspect() const;

    float z_near() const;

    float z_far() const;

    float diagonal() const;

    float radius() const;

    glm::vec3 center() const;

    glm::mat4 view() const;

    void set_view(const glm::mat4& view);

    glm::mat4 projection() const;

    void set_projection(float fovy, float aspect, float z_near, float z_far);

    glm::mat4 projection_view() const;

    void set_projection_view(float fovy, float aspect, float z_near, float z_far, const glm::mat4& view);

    glm::mat4 inverse_projection_view() const;

    glm::mat4 inverse_projection() const;

    glm::vec4 far_plane() const;

    glm::mat4 full_light_projection(const glm::mat4& light_projection_view_matrix, float& minZ, float& maxZ) const;

    glm::mat4 clip_light_projection(const glm::mat4& light_projection_view_matrix, float minZ, float maxZ) const;

    const std::array<glm::vec4, 8>& corners() const;

    bool contains_sphere(const glm::vec3& center, float radius) const;

private:
    float fovy_;
    float aspect_;
    float z_near_;
    float z_far_;
    float diagonal_;
    float radius_;
    glm::vec3 center_;
    glm::mat4 projection_;
    glm::mat4 view_;
    glm::mat4 projection_view_;
    glm::mat4 inverse_projection_view_;
    std::array<glm::vec4, 8> corners_;
    std::array<glm::vec4, 6> planes_;

    glm::mat4 light_projection_(const glm::mat4& light_projection_view_matrix, float& minZ, float& maxZ, bool updateZ) const;

    void rebuild_();
};
}

#endif // SIGMA_GRAPHICS_FRUSTUM_HPP

#include <sigma/frustum.hpp>

#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>
#include <limits>

namespace sigma {
frustum::frustum()
{
    set_projection_view(0.785398f, 1.0f, 0.1f, 100.0f, glm::mat4{});
}

frustum::frustum(float fovy, float aspect, float z_near, float z_far, const glm::mat4& view)
{
    set_projection_view(fovy, aspect, z_near, z_far, view);
}

float frustum::fovy() const
{
    return fovy_;
}

float frustum::aspect() const
{
    return aspect_;
}

float frustum::z_near() const
{
    return z_near_;
}

float frustum::z_far() const
{
    return z_far_;
}

float frustum::diagonal() const
{
    return diagonal_;
}

float frustum::radius() const
{
    return radius_;
}

glm::vec3 frustum::center() const
{
    return center_;
}

glm::mat4 frustum::view() const
{
    return view_;
}

void frustum::set_view(const glm::mat4& view)
{
    view_ = view;
    rebuild_();
}

glm::mat4 frustum::projection() const
{
    return projection_;
}

void frustum::set_projection(float fovy, float aspect, float z_near, float z_far)
{
    fovy_ = fovy;
    aspect_ = aspect;
    z_near_ = z_near;
    z_far_ = z_far;
    projection_ = glm::perspective(fovy_, aspect_, z_near_, z_far_);
    rebuild_();
}

glm::mat4 frustum::projection_view() const
{
    return projection_view_;
}

void frustum::set_projection_view(float fovy, float aspect, float z_near, float z_far, const glm::mat4& view)
{
    fovy_ = fovy;
    aspect_ = aspect;
    z_near_ = z_near;
    z_far_ = z_far;
    view_ = view;
    projection_ = glm::perspective(fovy_, aspect_, z_near_, z_far_);
    rebuild_();
}

glm::mat4 frustum::inverse_projection_view() const
{
    return inverse_projection_view_;
}

glm::mat4 frustum::inverse_projection() const
{
    // TODO pre compute this
    return glm::inverse(projection_);
}

glm::vec4 frustum::far_plane() const
{
    // http://www.txutxi.com/?p=444
    // http://gamedevs.org/uploads/fast-extraction-viewing-frustum-planes-from-world-view-projection-matrix.pdf

    // TODO pre compute this
    return glm::row(projection_view_, 3) - glm::row(projection_view_, 2);
}

glm::mat4 frustum::full_light_projection(const glm::mat4& light_projection_view_matrix, float& minZ, float& maxZ) const
{
    return light_projection_(light_projection_view_matrix, minZ, maxZ, true);
}

glm::mat4 frustum::clip_light_projection(const glm::mat4& light_projection_view_matrix, float minZ, float maxZ) const
{
    return light_projection_(light_projection_view_matrix, minZ, maxZ, false);
}

const std::array<glm::vec4, 8>& frustum::corners() const
{
    return corners_;
}

glm::mat4 frustum::light_projection_(const glm::mat4& light_projection_view_matrix, float& minZ, float& maxZ, bool updateZ) const
{
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();
    if (updateZ) {
        minZ = std::numeric_limits<float>::max();
        maxZ = std::numeric_limits<float>::min();
    }

    for (auto c : corners_) {
        auto v = light_projection_view_matrix * c;
        v /= v.w;
        minX = std::min(minX, v.x);
        maxX = std::max(maxX, v.x);
        minY = std::min(minY, v.y);
        maxY = std::max(maxY, v.y);
        if (updateZ) {
            minZ = std::min(minZ, v.z);
            maxZ = std::max(maxZ, v.z);
        }
    }

    return glm::ortho(minX, maxX, minY, maxY, -maxZ, -minZ);
}

void frustum::rebuild_()
{
    projection_view_ = projection_ * view_;
    inverse_projection_view_ = glm::inverse(projection_view_);

    // corners_[0] = { -1, -1, -1, 1 };
    // corners_[1] = { 1, -1, -1, 1 };
    // corners_[2] = { 1, 1, -1, 1 };
    // corners_[3] = { -1, 1, -1, 1 };
    //
    // corners_[4] = { -1, -1, 1, 1 };
    // corners_[5] = { 1, -1, 1, 1 };
    // corners_[6] = { 1, 1, 1, 1 };
    // corners_[7] = { -1, 1, 1, 1 };

    corners_[0] = { -1, -1, -1, 1 };
    corners_[1] = { -1, 1, -1, 1 };
    corners_[2] = { 1, 1, -1, 1 };
    corners_[3] = { 1, -1, -1, 1 };

    corners_[4] = { -1, -1, 1, 1 };
    corners_[5] = { -1, 1, 1, 1 };
    corners_[6] = { 1, 1, 1, 1 };
    corners_[7] = { 1, -1, 1, 1 };

    center_ = glm::vec3{ 0 };
    for (auto& c : corners_) {
        c = inverse_projection_view_ * c;
        c /= c.w;
        center_ += glm::vec3{ c };
    }
    center_ /= 8.0f;

    radius_ = std::numeric_limits<float>::min();
    for (auto c : corners_)
        radius_ = std::max(radius_, glm::distance(glm::vec3{ c }, center_));

    diagonal_ = glm::length(glm::vec3(corners_[6] - corners_[0]));
}
}

#include <sigma/graphics/frustum.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <algorithm>

namespace sigma {
namespace graphics {

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

    void frustum::rebuild_()
    {
        projection_view_ = projection_ * view_;
        inverse_projection_view_ = glm::inverse(projection_view_);

        glm::vec4 corners[] = {
            glm::vec4{ -1, -1, -1, 1 },
            glm::vec4{ -1, 1, -1, 1 },
            glm::vec4{ 1, 1, -1, 1 },
            glm::vec4{ 1, -1, -1, 1 },
            glm::vec4{ -1, -1, 1, 1 },
            glm::vec4{ -1, 1, 1, 1 },
            glm::vec4{ 1, 1, 1, 1 },
            glm::vec4{ 1, -1, 1, 1 }
        };

        center_ = glm::vec3{ 0 };
        for (auto& c : corners) {
            c = inverse_projection_view_ * c;
            c /= c.w;
            center_ += glm::vec3{ c };
        }
        center_ /= 8.0f;

        radius_ = std::numeric_limits<float>::min();
        for (auto c : corners)
            radius_ = std::max(radius_, glm::distance(glm::vec3{ c }, center_));

        diagonal_ = glm::length(glm::vec3(corners[6] - corners[0]));
    }
}
}

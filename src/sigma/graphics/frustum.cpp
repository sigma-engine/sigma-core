#include <sigma/graphics/frustum.hpp>

#include <glm/gtc/matrix_transform.hpp>

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

    void frustum::rebuild_()
    {
        projection_view_ = projection_ * view_;
    }
}
}

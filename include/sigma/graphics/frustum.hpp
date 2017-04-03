#ifndef SIGMA_GRAPHICS_FRUSTUM_HPP
#define SIGMA_GRAPHICS_FRUSTUM_HPP

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace sigma {
namespace graphics {
    class frustum {
    public:
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

        void rebuild_();
    };
}
}

#endif // SIGMA_GRAPHICS_FRUSTUM_HPP

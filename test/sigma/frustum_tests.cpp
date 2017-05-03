#include <gtest/gtest.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>

#include <array>

template <class T>
class frustum {
public:
    frustum(const T& fovy, const T& aspect, const T& z_near, const T& z_far)
        : matrix_(glm::perspective(fovy, aspect, z_near, z_far))
        , z_near_(z_near)
        , z_far_(z_far)
    {
        update_corners_();
    }

    frustum(const T& left, const T& right, const T& top, const T& bottom, const T& z_near, const T& z_far)
        : matrix_(glm::ortho(left, right, top, bottom, z_near, z_far))
        , z_near_(z_near)
        , z_far_(z_far)
    {
        update_corners_();
    }

    const std::array<glm::tvec3<T>, 8>& corners() const { return corners_; }

private:
    T z_near_;
    T z_far_;
    glm::tmat4x4<T> matrix_;
    std::array<glm::tvec3<T>, 8> corners_;

    void update_corners_()
    {
        static const glm::tvec4<T> corners[] = {
            glm::tvec4<T>{ -1, -1, -1, 1 },
            glm::tvec4<T>{ -1, 1, -1, 1 },
            glm::tvec4<T>{ 1, 1, -1, 1 },
            glm::tvec4<T>{ 1, -1, -1, 1 },
            glm::tvec4<T>{ -1, -1, 1, 1 },
            glm::tvec4<T>{ -1, 1, 1, 1 },
            glm::tvec4<T>{ 1, 1, 1, 1 },
            glm::tvec4<T>{ 1, -1, 1, 1 }
        };

        auto inverse = glm::inverse(matrix_);
        for (int i = 0; i < corners_.size(); ++i) {
            auto c = inverse * corners[i];
            c /= c.w;
            corners_[i] = glm::tvec3<T>{ c };
        }
    }
};

template <class T>
frustum<T> compute_light_frustum(const frustum<T>& camera, const glm::tmat4x4<T>& light_model_matrix)
{
    return camera;
}

TEST(frustum, perspective_corners_are_where_they_should_be)
{
    auto f = frustum<float>(glm::radians(90.0f), 1.0f, 2.0f, 100.0f);

    auto corners = f.corners();
    // Near plane

    // Bottom left
    EXPECT_NEAR(-2.0f, corners[0].x, 10e-5f);
    EXPECT_NEAR(-2.0f, corners[0].y, 10e-5f);
    EXPECT_NEAR(-2.0f, corners[0].z, 10e-5f);

    // Top left
    EXPECT_NEAR(-2.0f, corners[1].x, 10e-5f);
    EXPECT_NEAR(2.0f, corners[1].y, 10e-5f);
    EXPECT_NEAR(-2.0f, corners[1].z, 10e-5f);

    // Top right
    EXPECT_NEAR(2.0f, corners[2].x, 10e-5f);
    EXPECT_NEAR(2.0f, corners[2].y, 10e-5f);
    EXPECT_NEAR(-2.0f, corners[2].z, 10e-5f);

    // Bottom right
    EXPECT_NEAR(2.0f, corners[3].x, 10e-5f);
    EXPECT_NEAR(-2.0f, corners[3].y, 10e-5f);
    EXPECT_NEAR(-2.0f, corners[3].z, 10e-5f);

    // Far plane

    // Bottom left
    EXPECT_NEAR(-100.0f, corners[4].x, 10e-5f);
    EXPECT_NEAR(-100.0f, corners[4].y, 10e-5f);
    EXPECT_NEAR(-100.0f, corners[4].z, 10e-5f);

    // Top left
    EXPECT_NEAR(-100.0f, corners[5].x, 10e-5f);
    EXPECT_NEAR(100.0f, corners[5].y, 10e-5f);
    EXPECT_NEAR(-100.0f, corners[5].z, 10e-5f);
    // Top right
    EXPECT_NEAR(100.0f, corners[6].x, 10e-5f);
    EXPECT_NEAR(100.0f, corners[6].y, 10e-5f);
    EXPECT_NEAR(-100.0f, corners[6].z, 10e-5f);

    // Bottom right
    EXPECT_NEAR(100.0f, corners[7].x, 10e-5f);
    EXPECT_NEAR(-100.0f, corners[7].y, 10e-5f);
    EXPECT_NEAR(-100.0f, corners[7].z, 10e-5f);
}

TEST(frustum, orthographics_corners_are_where_they_should_be)
{
    auto f = frustum<float>(-40, 40, -30, 30, -33, 33);

    auto corners = f.corners();
    // Near plane

    // Bottom left
    EXPECT_NEAR(-40.0f, corners[0].x, 10e-5f);
    EXPECT_NEAR(-30.0f, corners[0].y, 10e-5f);
    EXPECT_NEAR(33.0f, corners[0].z, 10e-5f);

    // Top left
    EXPECT_NEAR(-40.0f, corners[1].x, 10e-5f);
    EXPECT_NEAR(30.0f, corners[1].y, 10e-5f);
    EXPECT_NEAR(33.0f, corners[1].z, 10e-5f);

    // Top right
    EXPECT_NEAR(40.0f, corners[2].x, 10e-5f);
    EXPECT_NEAR(30.0f, corners[2].y, 10e-5f);
    EXPECT_NEAR(33.0f, corners[2].z, 10e-5f);

    // Bottom right
    EXPECT_NEAR(40.0f, corners[3].x, 10e-5f);
    EXPECT_NEAR(-30.0f, corners[3].y, 10e-5f);
    EXPECT_NEAR(33.0f, corners[3].z, 10e-5f);

    // Far plane

    // Bottom left
    EXPECT_NEAR(-40.0f, corners[4].x, 10e-5f);
    EXPECT_NEAR(-30.0f, corners[4].y, 10e-5f);
    EXPECT_NEAR(-33.0f, corners[4].z, 10e-5f);

    // Top left
    EXPECT_NEAR(-40.0f, corners[5].x, 10e-5f);
    EXPECT_NEAR(30.0f, corners[5].y, 10e-5f);
    EXPECT_NEAR(-33.0f, corners[5].z, 10e-5f);
    // Top right
    EXPECT_NEAR(40.0f, corners[6].x, 10e-5f);
    EXPECT_NEAR(30.0f, corners[6].y, 10e-5f);
    EXPECT_NEAR(-33.0f, corners[6].z, 10e-5f);

    // Bottom right
    EXPECT_NEAR(40.0f, corners[7].x, 10e-5f);
    EXPECT_NEAR(-30.0f, corners[7].y, 10e-5f);
    EXPECT_NEAR(-33.0f, corners[7].z, 10e-5f);
}

// TEST(perspective_frustum, compute_light_frustrum)
// {
//     auto f = frustum<float>(glm::radians(90.0f), 1.0f, 2.0f, 100.0f);
//     auto light_model_matrix = glm::rotate(glm::radians(90.0f), glm::vec3{ 0.0f, 0.0f, 1.0f });
//
//     auto lf = compute_light_frustum(f, light_model_matrix);
//
//     auto corners = lf.corners();
//     // Near plane
//
//     // Bottom left
//     EXPECT_NEAR(-2.0f, corners[0].x, 10e-5f);
//     EXPECT_NEAR(-2.0f, corners[0].y, 10e-5f);
//     EXPECT_NEAR(-2.0f, corners[0].z, 10e-5f);
//
//     // Top left
//     EXPECT_NEAR(-2.0f, corners[1].x, 10e-5f);
//     EXPECT_NEAR(2.0f, corners[1].y, 10e-5f);
//     EXPECT_NEAR(-2.0f, corners[1].z, 10e-5f);
//
//     // Top right
//     EXPECT_NEAR(2.0f, corners[2].x, 10e-5f);
//     EXPECT_NEAR(2.0f, corners[2].y, 10e-5f);
//     EXPECT_NEAR(-2.0f, corners[2].z, 10e-5f);
//
//     // Bottom right
//     EXPECT_NEAR(2.0f, corners[3].x, 10e-5f);
//     EXPECT_NEAR(-2.0f, corners[3].y, 10e-5f);
//     EXPECT_NEAR(-2.0f, corners[3].z, 10e-5f);
//
//     // Far plane
//
//     // Bottom left
//     EXPECT_NEAR(-100.0f, corners[4].x, 10e-5f);
//     EXPECT_NEAR(-100.0f, corners[4].y, 10e-5f);
//     EXPECT_NEAR(-100.0f, corners[4].z, 10e-5f);
//
//     // Top left
//     EXPECT_NEAR(-100.0f, corners[5].x, 10e-5f);
//     EXPECT_NEAR(100.0f, corners[5].y, 10e-5f);
//     EXPECT_NEAR(-100.0f, corners[5].z, 10e-5f);
//     // Top right
//     EXPECT_NEAR(100.0f, corners[6].x, 10e-5f);
//     EXPECT_NEAR(100.0f, corners[6].y, 10e-5f);
//     EXPECT_NEAR(-100.0f, corners[6].z, 10e-5f);
//
//     // Bottom right
//     EXPECT_NEAR(100.0f, corners[7].x, 10e-5f);
//     EXPECT_NEAR(-100.0f, corners[7].y, 10e-5f);
//     EXPECT_NEAR(-100.0f, corners[7].z, 10e-5f);
// }

#include <sigma/frustum.hpp>

#include <gtest/gtest.h>

#include <glm/trigonometric.hpp>

TEST(frustum, perspective_corners_are_where_they_should_be)
{
    auto f = sigma::frustum(glm::radians(90.0f), 1.0f, 2.0f, 100.0f);

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

// TEST(frustum, orthographics_corners_are_where_they_should_be)
// {
//     auto f = sigma::frustum(-40, 40, -30, 30, -33, 33);
//
//     auto corners = f.corners();
//     // Near plane
//
//     // Bottom left
//     EXPECT_NEAR(-40.0f, corners[0].x, 10e-5f);
//     EXPECT_NEAR(-30.0f, corners[0].y, 10e-5f);
//     EXPECT_NEAR(33.0f, corners[0].z, 10e-5f);
//
//     // Top left
//     EXPECT_NEAR(-40.0f, corners[1].x, 10e-5f);
//     EXPECT_NEAR(30.0f, corners[1].y, 10e-5f);
//     EXPECT_NEAR(33.0f, corners[1].z, 10e-5f);
//
//     // Top right
//     EXPECT_NEAR(40.0f, corners[2].x, 10e-5f);
//     EXPECT_NEAR(30.0f, corners[2].y, 10e-5f);
//     EXPECT_NEAR(33.0f, corners[2].z, 10e-5f);
//
//     // Bottom right
//     EXPECT_NEAR(40.0f, corners[3].x, 10e-5f);
//     EXPECT_NEAR(-30.0f, corners[3].y, 10e-5f);
//     EXPECT_NEAR(33.0f, corners[3].z, 10e-5f);
//
//     // Far plane
//
//     // Bottom left
//     EXPECT_NEAR(-40.0f, corners[4].x, 10e-5f);
//     EXPECT_NEAR(-30.0f, corners[4].y, 10e-5f);
//     EXPECT_NEAR(-33.0f, corners[4].z, 10e-5f);
//
//     // Top left
//     EXPECT_NEAR(-40.0f, corners[5].x, 10e-5f);
//     EXPECT_NEAR(30.0f, corners[5].y, 10e-5f);
//     EXPECT_NEAR(-33.0f, corners[5].z, 10e-5f);
//     // Top right
//     EXPECT_NEAR(40.0f, corners[6].x, 10e-5f);
//     EXPECT_NEAR(30.0f, corners[6].y, 10e-5f);
//     EXPECT_NEAR(-33.0f, corners[6].z, 10e-5f);
//
//     // Bottom right
//     EXPECT_NEAR(40.0f, corners[7].x, 10e-5f);
//     EXPECT_NEAR(-30.0f, corners[7].y, 10e-5f);
//     EXPECT_NEAR(-33.0f, corners[7].z, 10e-5f);
// }

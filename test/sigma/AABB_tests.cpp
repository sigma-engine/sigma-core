#include <sigma/AABB.hpp>

#include <gtest/gtest.h>

TEST(AABB, contains_point_with_point_to_right_is_false)
{
    std::pair<sigma::AABB, glm::vec3> box_and_points[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { 0.55f, 0, 0 } },
        { { { 1, 0, 0 }, { 1, 1, 1 } }, { 1.55f, 0, 0 } },
        { { { 0, 0, 0 }, { 2, 1, 1 } }, { 1.05f, 0, 0 } },
        { { { 1, 0, 0 }, { 2, 1, 1 } }, { 2.05f, 0, 0 } }
    };

    for (const auto& test_case : box_and_points) {
        EXPECT_FALSE(test_case.first.contains(test_case.second));
    }
}

TEST(AABB, contains_point_with_point_to_left_is_false)
{
    std::pair<sigma::AABB, glm::vec3> box_and_points[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { -0.55f, 0, 0 } },
        { { { 1, 0, 0 }, { 1, 1, 1 } }, { -1.55f, 0, 0 } },
        { { { 0, 0, 0 }, { 2, 1, 1 } }, { -1.05f, 0, 0 } },
        { { { 1, 0, 0 }, { 2, 1, 1 } }, { -2.05f, 0, 0 } }
    };

    for (const auto& test_case : box_and_points) {
        EXPECT_FALSE(test_case.first.contains(test_case.second));
    }
}

TEST(AABB, contains_point_with_point_above_is_false)
{
    std::pair<sigma::AABB, glm::vec3> box_and_points[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { 0, 0.55f, 0 } },
        { { { 0, 1, 0 }, { 1, 1, 1 } }, { 0, 1.55f, 0 } },
        { { { 0, 0, 0 }, { 1, 2, 1 } }, { 0, 1.05f, 0 } },
        { { { 0, 1, 0 }, { 1, 2, 1 } }, { 0, 2.05f, 0 } }
    };

    for (const auto& test_case : box_and_points) {
        EXPECT_FALSE(test_case.first.contains(test_case.second));
    }
}

TEST(AABB, contains_point_with_point_below_is_false)
{
    std::pair<sigma::AABB, glm::vec3> box_and_points[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { 0, -0.55f, 0 } },
        { { { 0, 1, 0 }, { 1, 1, 1 } }, { 0, -1.55f, 0 } },
        { { { 0, 0, 0 }, { 1, 2, 1 } }, { 0, -1.05f, 0 } },
        { { { 0, 1, 0 }, { 1, 2, 1 } }, { 0, -2.05f, 0 } }
    };

    for (const auto& test_case : box_and_points) {
        EXPECT_FALSE(test_case.first.contains(test_case.second));
    }
}

TEST(AABB, contains_point_with_point_in_front_is_false)
{
    std::pair<sigma::AABB, glm::vec3> box_and_points[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { 0, 0, 0.55f } },
        { { { 0, 0, 1 }, { 1, 1, 1 } }, { 0, 0, 1.55f } },
        { { { 0, 0, 0 }, { 1, 1, 2 } }, { 0, 0, 1.05f } },
        { { { 0, 0, 1 }, { 1, 1, 2 } }, { 0, 0, 2.05f } }
    };

    for (const auto& test_case : box_and_points) {
        EXPECT_FALSE(test_case.first.contains(test_case.second));
    }
}

TEST(AABB, contains_point_with_point_behind_is_false)
{
    std::pair<sigma::AABB, glm::vec3> box_and_points[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { 0, 0, -0.55f } },
        { { { 0, 0, 1 }, { 1, 1, 1 } }, { 0, 0, -1.55f } },
        { { { 0, 0, 0 }, { 1, 1, 2 } }, { 0, 0, -1.05f } },
        { { { 0, 0, 1 }, { 1, 1, 2 } }, { 0, 0, -2.05f } }
    };

    for (const auto& test_case : box_and_points) {
        EXPECT_FALSE(test_case.first.contains(test_case.second));
    }
}

TEST(AABB, contains_point_with_point_in_is_true)
{
    std::pair<sigma::AABB, glm::vec3> box_and_points[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { 0, 0, 0 } },
        { { { 0, 0, 1 }, { 1, 1, 1 } }, { 0, 0, 1.5f } },
        { { { 0, 0, 0 }, { 1, 1, 2 } }, { 0, 0, 1.0f } },
        { { { 0, 0, 1 }, { 1, 1, 2 } }, { 0, 0, 2.0f } }
    };

    for (const auto& test_case : box_and_points) {
        EXPECT_TRUE(test_case.first.contains(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_not_touching_the_right_face_is_false)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { -1.05, 0, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 2, 1, 1 } }, { { -1.55, 0, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 2, 1, 1 } }, { { -2.05, 0, 0 }, { 2, 1, 1 } } },
        { { { 1, 0, 0 }, { 2, 1, 1 } }, { { -1.05, 0, 0 }, { 2, 1, 1 } } }
    };

    for (const auto& test_case : boxes) {
        EXPECT_FALSE(test_case.first.collides(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_touching_the_right_face_is_true)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { -1.0, 0, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 2, 1, 1 } }, { { -1.5, 0, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 2, 1, 1 } }, { { -2.0, 0, 0 }, { 2, 1, 1 } } },
        { { { 1, 0, 0 }, { 2, 1, 1 } }, { { -1.0, 0, 0 }, { 2, 1, 1 } } }
    };

    for (const auto& test_case : boxes) {
        EXPECT_TRUE(test_case.first.collides(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_not_touching_the_left_face_is_false)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { 1.05, 0, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 2, 1, 1 } }, { { 1.55, 0, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 2, 1, 1 } }, { { 2.05, 0, 0 }, { 2, 1, 1 } } },
        { { { 1, 0, 0 }, { 2, 1, 1 } }, { { 3.05, 0, 0 }, { 2, 1, 1 } } }
    };

    for (const auto& test_case : boxes) {
        EXPECT_FALSE(test_case.first.collides(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_touching_the_left_face_is_true)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { 1.0, 0, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 2, 1, 1 } }, { { 1.5, 0, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 2, 1, 1 } }, { { 2.0, 0, 0 }, { 2, 1, 1 } } },
        { { { 1, 0, 0 }, { 2, 1, 1 } }, { { 3.0, 0, 0 }, { 2, 1, 1 } } }
    };

    for (const auto& test_case : boxes) {
        EXPECT_TRUE(test_case.first.collides(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_not_touching_the_bottom_face_is_false)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { 0, -1.05, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 2, 1 } }, { { 0, -1.55, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 2, 1 } }, { { 0, -2.05, 0 }, { 1, 2, 1 } } },
        { { { 0, 1, 0 }, { 1, 2, 1 } }, { { 0, -1.05, 0 }, { 1, 2, 1 } } }
    };

    for (const auto& test_case : boxes) {
        EXPECT_FALSE(test_case.first.collides(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_touching_the_bottom_face_is_true)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { 0, -1.0, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 2, 1 } }, { { 0, -1.5, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 2, 1 } }, { { 0, -2.0, 0 }, { 1, 2, 1 } } },
        { { { 0, 1, 0 }, { 1, 2, 1 } }, { { 0, -1.0, 0 }, { 1, 2, 1 } } }
    };

    for (const auto& test_case : boxes) {
        EXPECT_TRUE(test_case.first.collides(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_not_touching_the_top_face_is_false)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { 0, 1.05, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 2, 1 } }, { { 0, 1.55, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 2, 1 } }, { { 0, 2.05, 0 }, { 1, 2, 1 } } },
        { { { 0, 1, 0 }, { 1, 2, 1 } }, { { 0, 3.05, 0 }, { 1, 2, 1 } } }
    };

    for (const auto& test_case : boxes) {
        EXPECT_FALSE(test_case.first.collides(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_touching_the_top_face_is_true)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { 0, 1.0, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 2, 1 } }, { { 0, 1.5, 0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 2, 1 } }, { { 0, 2.0, 0 }, { 1, 2, 1 } } },
        { { { 0, 1, 0 }, { 1, 2, 1 } }, { { 0, 3.0, 0 }, { 1, 2, 1 } } }
    };

    for (const auto& test_case : boxes) {
        EXPECT_TRUE(test_case.first.collides(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_not_touching_the_back_face_is_false)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { 0, 0, -1.05 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 1, 2 } }, { { 0, 0, -1.55 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 1, 2 } }, { { 0, 0, -2.05 }, { 1, 1, 2 } } },
        { { { 0, 0, 1 }, { 1, 1, 2 } }, { { 0, 0, -1.05 }, { 1, 1, 2 } } }
    };

    for (const auto& test_case : boxes) {
        EXPECT_FALSE(test_case.first.collides(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_not_touching_the_front_face_is_false)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { 0, 0, 1.0 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 1, 2 } }, { { 0, 0, 1.5 }, { 1, 1, 1 } } },
        { { { 0, 0, 0 }, { 1, 1, 2 } }, { { 0, 0, 2.0 }, { 1, 1, 2 } } },
        { { { 0, 0, 1 }, { 1, 1, 2 } }, { { 0, 0, 2.0 }, { 1, 1, 2 } } }
    };

    for (const auto& test_case : boxes) {
        EXPECT_TRUE(test_case.first.collides(test_case.second));
    }
}

TEST(AABB, collides_with_AABB_inside_is_true)
{
    std::pair<sigma::AABB, sigma::AABB> boxes[] = {
        { { { 0, 0, 0 }, { 1, 1, 1 } }, { { 0, 0, 0 }, { 0.5f, 0.5f, 0.5f } } },
        { { { 5, 5, 5 }, { 1, 1, 1 } }, { { 5, 5, 5 }, { 0.5f, 0.5f, 0.5f } } },
    };

    for (const auto& test_case : boxes) {
        EXPECT_TRUE(test_case.first.collides(test_case.second));
    }
}

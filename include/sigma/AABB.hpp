#ifndef SIGMA_AABB_HPP
#define SIGMA_AABB_HPP

#include <glm/vec3.hpp>

#include <limits>

namespace sigma {
class AABB {
    glm::vec3 center_;
    glm::vec3 half_size_;

    glm::vec3 min_;
    glm::vec3 max_;

public:
    AABB(const glm::vec3& center = { 0.0f, 0.0f, 0.0f }, const glm::vec3& size = { 1.0f, 1.0f, 1.0f })
        : center_ { center }
        , half_size_ { size / 2.0f }
    {
        recalculate();
    }

    glm::vec3 center() const noexcept
    {
        return center_;
    }

    void set_center(const glm::vec3& center) noexcept
    {
        this->center_ = center;
        recalculate();
    }

    glm::vec3 size() const noexcept
    {
        return half_size_ * 2.0f;
    }

    AABB minkowski_difference(const AABB& other) const noexcept
    {
        glm::vec3 tl = min_ - other.max_;
        glm::vec3 new_half_size = (half_size_ + other.half_size_);
        return {
            tl + new_half_size, 2.0f * new_half_size
        };
    }

    bool contains(const glm::vec3& p) const noexcept
    {
        return (min_.x <= p.x && p.x <= max_.x) && (min_.y <= p.y && p.y <= max_.y) && (min_.z <= p.z && p.z <= max_.z);
    }

    glm::vec3 closest_point_on_surface(const glm::vec3& p)
    {
        assert(contains(p));

        const glm::vec3 surface_points[] = {
            { min_.x, p.y, p.z },
            { max_.x, p.y, p.z },

            { p.x, min_.y, p.z },
            { p.x, max_.y, p.z },

            { p.x, p.y, min_.z },
            { p.x, p.y, max_.z },
        };

        float min_dist = std::numeric_limits<float>::max();
        glm::vec3 output;
        for (const auto& sp : surface_points) {
            auto dist = (sp - p).length();
            if (dist < min_dist) {
                min_dist = dist;
                output = sp;
            }
        }
        return output;
    }

    bool collides(const AABB& other) const
    {
        AABB diff = minkowski_difference(other);
        return diff.contains({ 0, 0, 0 });
    }

    bool collides(const AABB& other, glm::vec3& penetration) const
    {
        AABB diff = minkowski_difference(other);
        if (diff.contains({ 0, 0, 0 })) {
            penetration = diff.closest_point_on_surface({ 0, 0, 0 });
            return true;
        }
        return false;
    }

private:
    void recalculate()
    {
        min_ = center_ - half_size_;
        max_ = center_ + half_size_;
    }
};
}

#endif // SIGMA_AABB_HPP

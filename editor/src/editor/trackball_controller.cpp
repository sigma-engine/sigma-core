#include <editor/trackball_controller.hpp>
#include <math/utils.hpp>

namespace sigmafive {
    namespace editor {
        trackball_controller::trackball_controller(float radius)
                : radius_(radius) {
        }

        float trackball_controller::radius() const {
            return this->radius_;
        }

        void trackball_controller::set_radius(float radius) {
            this->radius_ = radius;
        }

        quaternionf trackball_controller::rotation() const {
            return rotation_;
        }

        float4x4 trackball_controller::matrix() const {
            float4x4 m(rotation_);
            m[3] = float4(position_, 1.0f);//TODO translate function in float4x4
            return m;
        }

        void trackball_controller::resize(int2 size) {
            this->size_ = size;
        }

        float3 trackball_controller::project_to_sphere(int2 point) const {
            auto p = float2(remap<float>(point.x, 0.0f, size_.x, -1.0f, 1.0f),
                    -remap<float>(point.y, 0.0f, size_.y, -1.0f, 1.0f));

            auto d = p.length();
            if (d < radius_ * 0.70710678118654752440) {
                return float3(p, std::sqrt(radius_ * radius_ - d * d));
            }
            else {
                auto t = radius_ / 1.41421356237309504880f;
                return float3(p, t * t / d);
            }
        }

        void trackball_controller::mouse_down(int2 location) {
            lastLocation_ = currentLocation_;
            currentLocation_ = location;
        }

        void trackball_controller::mouse_up(int2 location) {
            lastLocation_ = currentLocation_;
            currentLocation_ = location;
        }

        void trackball_controller::mouse_move(int2 location) {
            lastLocation_ = currentLocation_;
            currentLocation_ = location;

            auto p1 = project_to_sphere(lastLocation_);
            auto p2 = project_to_sphere(currentLocation_);

            auto d = p1 - p2;
            auto t = clamp(d.length() / (2 * radius_), -1.0f, 1.0f);

            auto axis = p2 * p1;
            auto angle = 2 * std::asin(t);

            rotation_ = rotation_ * quaternionf::from_axis_angle(axis, angle);
            rotation_.normalize();
        }

        void trackball_controller::mouse_scroll(int direction) {
            position_ += float3(0, 0, .01f * direction);
        }

    }
}
#include <editor/trackball_controller.hpp>
#include <sigmafive/math/utils.hpp>
#include <sigmafive/math/mat4.hpp>

namespace sigmafive {
    namespace editor {
        trackball_controller::trackball_controller(float radius)
                : radius_(radius),rotating_(false),panning_(false) {
        }

        float trackball_controller::radius() const {
            return this->radius_;
        }

        void trackball_controller::set_radius(float radius) {
            this->radius_ = radius;
        }

        float4x4 trackball_controller::matrix() const {
            return float4x4::rotation(rotation_) * float4x4::translation(position_);
        }

        void trackball_controller::begin_rotate(float2 location) {
            lastLocation_ = currentLocation_;
            currentLocation_ = location;
            rotating_ = true;
        }

        void trackball_controller::begin_pan() {
            rotating_ = false;
            panning_ = true;
        }

        void trackball_controller::update(float2 location) {
            lastLocation_ = currentLocation_;
            currentLocation_ = location;
            if(panning_) {
                auto inv_rotation = rotation_.get_inverse();
                auto view_vector = inv_rotation.rotate(-float3::OUTWARD*position_.length());
                auto right_vector = inv_rotation.rotate(float3::RIGHT);
                auto up_vector = inv_rotation.rotate(float3::UP);

                //pan_rate comes from simple triangle trigonometry
                //if it is less than one make it one so we dont get
                //stuck at the origin
                auto pan_rate = clamp(std::abs(view_vector.z),1.0f,std::numeric_limits<float>::max());

                //TODO this panning method is a little weird
                //blender moves the rotation orign when it
                //pans an this moves  the camera
                auto dp = currentLocation_-lastLocation_;
                position_ += pan_rate * right_vector * dp.x;
                position_ += pan_rate * up_vector    * dp.y;
            }
            else if(rotating_) {
                auto p1 = project_to_sphere(lastLocation_);
                auto p2 = project_to_sphere(currentLocation_);

                auto d = p2 - p1;
                auto tx = clamp(d.x / (2 * radius_), -1.0f, 1.0f);
                auto ty = clamp(d.y / (2 * radius_), -1.0f, 1.0f);

                //find the yaw angle filp the sign if we are upside down
                auto ax = -2 * std::asin(tx) * (std::signbit((rotation_.rotate(float3::UP)).y)? 1 : -1);

                //find the pitch angle
                auto ay = -2 * std::asin(ty);

                rotation_ =  quaternionf::from_axis_angle(float3::RIGHT,ay) * rotation_ * quaternionf::from_axis_angle(float3::UP,ax);
                rotation_.normalize();
            }
        }

        void trackball_controller::end_rotate(float2 location) {
            lastLocation_ = currentLocation_;
            currentLocation_ = location;
            rotating_ = false;
        }

        void trackball_controller::end_pan() {
            panning_ = false;
        }

        void trackball_controller::zoom(int direction) {
            if(!rotating_ && ! panning_) {
                auto inv_rotation = rotation_.get_inverse();
                position_ += inv_rotation.rotate((.01f * direction * float3::OUTWARD));
            }
        }

        float3 trackball_controller::project_to_sphere(float2 point) const {
            auto d = point.length();
            if (d < radius_ * 0.70710678118654752440) {
                return float3(point, std::sqrt(radius_ * radius_ - d * d));
            }
            else {
                auto t = radius_ / 1.41421356237309504880f;
                return float3(point, t * t / d);
            }
        }
    }
}

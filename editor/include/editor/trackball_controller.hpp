#ifndef SIGMAFIVE_EDITOR_TRACKBALL_CONTROLLER_HPP
#define SIGMAFIVE_EDITOR_TRACKBALL_CONTROLLER_HPP

#include <editor/config.hpp>

#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/vec3.hpp>
#include <sigmafive/math/mat4.hpp>
#include <sigmafive/math/quaternion.hpp>

namespace sigmafive {
    namespace editor {

        class EDITOR_API trackball_controller {
        public:
            trackball_controller(float radius);

            float radius() const;

            void set_radius(float radius);

            float4x4 matrix() const;

            void begin_rotate(float2 location);

            void begin_pan();

            void update(float2 location);

            void end_rotate(float2 location);

            void end_pan();

            void zoom(int direction);

        private:
            float3 project_to_sphere(float2 point) const;

            float radius_;
            float2 currentLocation_, lastLocation_;
            float3 position_;
            quaternionf rotation_;
            bool rotating_;
            bool panning_;
        };
    }
}

#endif // SIGMAFIVE_EDITOR_TRACKBALL_CONTROLLER_HPP

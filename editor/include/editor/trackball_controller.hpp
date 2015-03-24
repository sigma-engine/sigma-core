#ifndef SIGMAFIVE_EDITOR_TRACKBALL_CONTROLLER_HPP
#define SIGMAFIVE_EDITOR_TRACKBALL_CONTROLLER_HPP

#include <math/vec2.hpp>
#include <math/vec3.hpp>
#include <math/mat4.hpp>
#include <math/quaternion.hpp>

namespace sigmafive {
    namespace editor {

        class trackball_controller {
        public:
            trackball_controller(float radius);

            float radius() const;

            void set_radius(float radius);

            quaternionf rotation() const;

            float4x4 matrix() const;

            void resize(int2 size);

            float3 project_to_sphere(int2 point) const;

            void mouse_down(int2 location);

            void mouse_up(int2 location);

            void mouse_move(int2 location);

            void mouse_scroll(int direction);
        private:
            float radius_;
            int2 size_;
            int2 currentLocation_, lastLocation_;
            float3 position_;
            quaternionf rotation_;
        };
    }
}

#endif // SIGMAFIVE_EDITOR_TRACKBALL_CONTROLLER_HPP

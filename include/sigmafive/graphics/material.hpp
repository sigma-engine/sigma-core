#ifndef SIGMAFIVE_ENGINE_GRAPHICS_MATERIAL_HPP
#define SIGMAFIVE_ENGINE_GRAPHICS_MATERIAL_HPP

#include <sigmafive/config.hpp>
#include <sigmafive/component.hpp>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <sigmafive/util/glm_serialize.hpp>

#include <array>
#include <vector>
#include <boost/serialization/array.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/vector.hpp>

namespace sigmafive {
    namespace graphics {
        class SIGMAFIVE_API material {
        public:
            // TODO fill this out
        private:
            friend class boost::serialization::access;
            template<class Archive>
            void serialize(Archive & ar, const unsigned int version) {
                // TODO fill this out
            }

            // TODO fill this out
        };

        class material_cache {
        public:
            // TODO fill this out
        private:
        };
    }
}

#endif //SIGMAFIVE_ENGINE_GRAPHICS_MATERIAL_HPP

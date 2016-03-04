#ifndef SIGMA_FIVE_ENGINE_UTIL_GLM_SERIALIZE_HPP
#define SIGMA_FIVE_ENGINE_UTIL_GLM_SERIALIZE_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace boost {
namespace serialization {
    template <class Archive>
    void serialize(Archive& ar, glm::vec2& v, const unsigned int version)
    {
        ar& v.x;
        ar& v.y;
    }

    template <class Archive>
    void serialize(Archive& ar, glm::vec3& v, const unsigned int version)
    {
        ar& v.x;
        ar& v.y;
        ar& v.z;
    }

    template <class Archive>
    void serialize(Archive& ar, glm::vec4& v, const unsigned int version)
    {
        ar& v.x;
        ar& v.y;
        ar& v.z;
        ar& v.w;
    }
}
}

#endif //SIGMA_FIVE_ENGINE_UTIL_GLM_SERIALIZE_HPP

#ifndef SIGMA_ENGINE_ENGINE_GLM_SERIALIZE_HPP
#define SIGMA_ENGINE_ENGINE_GLM_SERIALIZE_HPP

#include <sigma/config.hpp>

#include <glm/mat2x2.hpp>
#include <glm/mat2x3.hpp>
#include <glm/mat2x4.hpp>
#include <glm/mat3x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x2.hpp>
#include <glm/mat4x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace cereal {
    template <class Archive, typename T, glm::precision P>
    void serialize(Archive& ar, glm::tvec2<T, P>& v)
    {
        ar(v.x,
        v.y);
    }

    template <class Archive, typename T, glm::precision P>
    void serialize(Archive& ar, glm::tvec3<T, P>& v)
    {
        ar(v.x,
        v.y,
        v.z);
    }

    template <class Archive, typename T, glm::precision P>
    void serialize(Archive& ar, glm::tvec4<T, P>& v)
    {
        ar(v.x,
        v.y,
        v.z,
        v.w);
    }

    template <class Archive, typename T, glm::precision P>
    void serialize(Archive& ar, glm::tquat<T, P>& v)
    {
        ar(v.x,
        v.y,
        v.z,
        v.w);
    }

    template <class Archive, typename P>
    void serialize(Archive& ar, glm::tmat2x2<P>& v)
    {
        ar(v[0],
        v[1]);
    }

    template <class Archive, typename P>
    void serialize(Archive& ar, glm::tmat2x3<P>& v)
    {
        ar(v[0],
        v[1],
        v[2]);
    }

    template <class Archive, typename P>
    void serialize(Archive& ar, glm::tmat2x4<P>& v)
    {
        ar(v[0],
        v[1],
        v[2],
        v[3]);
    }

    template <class Archive, typename P>
    void serialize(Archive& ar, glm::tmat3x2<P>& v)
    {
        ar(v[0],
        v[1]);
    }

    template <class Archive, typename P>
    void serialize(Archive& ar, glm::tmat3x3<P>& v)
    {
        ar(v[0],
        v[1],
        v[2]);
    }

    template <class Archive, typename P>
    void serialize(Archive& ar, glm::tmat3x4<P>& v)
    {
        ar(v[0],
        v[1],
        v[2],
        v[3]);
    }

    template <class Archive, typename P>
    void serialize(Archive& ar, glm::tmat4x2<P>& v)
    {
        ar(v[0],
        v[1]);
    }

    template <class Archive, typename P>
    void serialize(Archive& ar, glm::tmat4x3<P>& v)
    {
        ar(v[0],
        v[1],
        v[2]);
    }

    template <class Archive, typename P>
    void serialize(Archive& ar, glm::tmat4x4<P>& v)
    {
        ar(v[0],
        v[1],
        v[2],
        v[3]);
    }
}

#endif // SIGMA_ENGINE_ENGINE_GLM_SERIALIZE_HPP

#ifndef SIGMA_ENGINE_JSON_CONVERSION_HPP
#define SIGMA_ENGINE_JSON_CONVERSION_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/handle.hpp>
#include <sigma/world.hpp>

#include <json/json.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/hana/at_key.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/keys.hpp>

#include <vector>

namespace sigma {
namespace json {
    namespace detial {
        template <class>
        struct type_traits;
    }

    template <class T>
    bool from_json(const Json::Value& source, T& output)
    {
        return detial::type_traits<T>::from(source, output);
    }

    namespace detial {
        template <>
        struct type_traits<bool> {
            static bool from(const Json::Value& source, bool& output)
            {
                if (source.isConvertibleTo(Json::booleanValue)) {
                    output = source.asBool();
                    return true;
                }
                return false;
            }
        };

        template <>
        struct type_traits<int> {
            static bool from(const Json::Value& source, int& output)
            {
                if (source.isConvertibleTo(Json::intValue)) {
                    output = source.asInt();
                    return true;
                }
                return false;
            }
        };
        template <>
        struct type_traits<unsigned long int> {
            static bool from(const Json::Value& source, unsigned long int& output)
            {
                if (source.isConvertibleTo(Json::uintValue)) {
                    output = source.asUInt64();
                    return true;
                }
                return false;
            }
        };

        template <>
        struct type_traits<float> {
            static bool from(const Json::Value& source, float& output)
            {
                if (source.isConvertibleTo(Json::realValue)) {
                    output = float(source.asDouble());
                    return true;
                }
                return false;
            }
        };

        template <>
        struct type_traits<glm::vec2> {
            static bool from(const Json::Value& source, glm::vec2& output)
            {
                // TODO support x,y
                return source.isArray() && source.size() == 2 && from_json(source[0], output.x) && from_json(source[1], output.y);
            }
        };

        template <>
        struct type_traits<glm::vec3> {
            static bool from(const Json::Value& source, glm::vec3& output)
            {
                // TODO support x,y,z
                return source.isArray() && source.size() == 3 && from_json(source[0], output.x) && from_json(source[1], output.y) && from_json(source[2], output.z);
            }
        };

        template <>
        struct type_traits<glm::vec4> {
            static bool from(const Json::Value& source, glm::vec4& output)
            {
                // TODO support w,x,y,z
                return source.isArray() && source.size() == 4 && from_json(source[0], output.w) && from_json(source[1], output.x) && from_json(source[2], output.y) && from_json(source[3], output.z);
            }
        };

        template <>
        struct type_traits<glm::quat> {
            static bool from(const Json::Value& source, glm::quat& output)
            {
                if (source.isArray()) {
                    if (source.size() == 4) {
                        return from_json(source[0], output.w) && from_json(source[1], output.x) && from_json(source[2], output.y) && from_json(source[3], output.z);
                    } else if (source.size() == 3) {
                        glm::vec3 e;
                        if (from_json(source, e)) {
                            output = glm::quat{ glm::radians(e) };
                            return true;
                        }
                    }
                    return false;
                } else {
                    if (!source.isMember("x") || !source.isMember("y") || !source.isMember("z"))
                        return false;
                    if (source.isMember("w")) {
                        return from_json(source["w"], output.w) && from_json(source["x"], output.x) && from_json(source["y"], output.y) && from_json(source["z"], output.z);
                    } else {
                        glm::vec3 e;
                        if (from_json(source, e)) {
                            output = glm::quat{ glm::radians(e) };
                            return true;
                        }
                    }
                }
                return false;
            }
        };

        template <>
        struct type_traits<glm::mat2> {
            static bool from(const Json::Value& source, glm::mat2& output)
            {
                return source.isArray() && source.size() == 2 && from_json(source[0], output[0]) && from_json(source[1], output[1]);
            }
        };

        template <>
        struct type_traits<glm::mat3> {
            static bool from(const Json::Value& source, glm::mat3& output)
            {
                return source.isArray() && source.size() == 3 && from_json(source[0], output[0]) && from_json(source[1], output[1]) && from_json(source[2], output[2]);
            }
        };

        template <>
        struct type_traits<glm::mat4> {
            static bool from(const Json::Value& source, glm::mat4& output)
            {
                return source.isArray() && source.size() == 4 && from_json(source[0], output[0]) && from_json(source[1], output[1]) && from_json(source[2], output[2]) && from_json(source[3], output[3]);
            }
        };

        template <>
        struct type_traits<boost::filesystem::path> {
            static bool from(const Json::Value& source, boost::filesystem::path& output)
            {
                output = source.asString();
                return true;
            }
        };

        template <class TagType>
        struct type_traits<resource::handle<TagType>> {
            static bool from(const Json::Value& source, resource::handle<TagType>& output)
            {
                if (source.isConvertibleTo(Json::uintValue)) {
                    output.index = source.asUInt();
                    output.version = 0; // TODO version
                    return true;
                }
                return false;
            }
        };

        template <class T, long int N>
        struct type_traits<T[N]> {
            static bool from(const Json::Value& source, T* output)
            {
                for (long int i = 0; i < N; ++i) {
                    if (!from_json(source[(int)i], output[i]))
                        return false;
                }
                return true;
            }
        };

        template <class T>
        struct type_traits<std::vector<T>> {
            static bool from(const Json::Value& source, std::vector<T>& output)
            {
                output.resize(source.size());
                for (std::size_t i = 0; i < output.size(); ++i) {
                    if (!from_json(source[(int)i], output[i]))
                        return false;
                }
                return true;
            }
        };

        template <>
        struct type_traits<sigma::graphics::texture_filter> {
            static bool from(const Json::Value& source, sigma::graphics::texture_filter& output)
            {
                auto lower = boost::to_upper_copy(source.asString());
                if (lower == "LINEAR")
                    output = graphics::texture_filter::LINEAR;
                else if (lower == "NEAREST")
                    output = graphics::texture_filter::NEAREST;
                else if (lower == "NONE")
                    output = graphics::texture_filter::NONE;
                else
                    return false;

                return true;
            }
        };

        template <>
        struct type_traits<sigma::graphics::texture_format> {
            static bool from(const Json::Value& source, sigma::graphics::texture_format& output)
            {
                auto lower = boost::to_upper_copy(source.asString());
                if (lower == "RGB8")
                    output = sigma::graphics::texture_format::RGB8;
                else if (lower == "RGBA8")
                    output = sigma::graphics::texture_format::RGBA8;
                else if (lower == "RGB32F")
                    output = sigma::graphics::texture_format::RGB32F;
                else
                    return false;
                return true;
            }
        };

        template <class T>
        struct type_traits {
            static bool from(const Json::Value& source, T& output)
            {
                bool good = true;
                boost::hana::for_each(boost::hana::keys(output), [&](auto key) {
                    auto& member = boost::hana::at_key(output, key);
                    using member_type = std::remove_reference_t<decltype(member)>;
                    if (source.isMember(key.c_str())) {
                        if (!type_traits<member_type>::from(source[key.c_str()], member))
                            good = false;
                    }
                });
                return true;
            }
        };

        template <class... Components>
        struct type_traits<sigma::world<Components...>> {
            template <class T>
            struct component_tag {
                using component_type = T;
            };

            template <class Func>
            static void foreach_component(Func f)
            {
                auto l = { (f(component_tag<Components>{}), 0)... };
                (void)l;
            }

            static bool from(const Json::Value& source, sigma::world<Components...>& output)
            {
                bool good = true;
                for (auto entity : source.getMemberNames()) {
                    auto e = output.create();
                    auto& src = source[entity];
                    foreach_component([&](auto comp_tag) {
                        using component_type = typename decltype(comp_tag)::component_type;
                        if (src.isMember(component_name(component_type))) {
                            component_type& cmp = output.template add<component_type>(e);
                            if (!type_traits<component_type>::from(src[component_name(component_type)], cmp))
                                good = false;
                        }
                    });
                }
                return good;
            }
        };
    }
}
}

#endif // SIGMA_ENGINE_JSON_CONVERSION_HPP

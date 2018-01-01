#ifndef SIGMA_ENGINE_JSON_CONVERSION_HPP
#define SIGMA_ENGINE_JSON_CONVERSION_HPP

#include <sigma/component.hpp>
#include <sigma/config.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/graphics/texture.hpp>
#include <sigma/resource/resource.hpp>
#include <sigma/tools/resource_hash.hpp>

#include <json/json.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/hana/at_key.hpp>
#include <boost/hana/for_each.hpp>
#include <boost/hana/keys.hpp>
#include <boost/hana/length.hpp>

#include <vector>

namespace sigma {
namespace json {
    namespace detial {
        template <class>
        struct type_traits;
    }

    template <class Context, class T>
    bool from_json(Context& ctx, const Json::Value& source, T& output)
    {
        return detial::type_traits<T>::from(ctx, source, output);
    }

    namespace detial {
        template <>
        struct type_traits<bool> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, bool& output)
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
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, int& output)
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
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, unsigned long int& output)
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
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, float& output)
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
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, glm::vec2& output)
            {
                // TODO support x,y
                return source.isArray() && source.size() == 2 && from_json(ctx, source[0], output.x) && from_json(ctx, source[1], output.y);
            }
        };

        template <>
        struct type_traits<glm::vec3> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, glm::vec3& output)
            {
                // TODO support x,y,z
                return source.isArray() && source.size() == 3 && from_json(ctx, source[0], output.x) && from_json(ctx, source[1], output.y) && from_json(ctx, source[2], output.z);
            }
        };

        template <>
        struct type_traits<glm::vec4> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, glm::vec4& output)
            {
                // TODO support w,x,y,z
                return source.isArray() && source.size() == 4 && from_json(ctx, source[0], output.w) && from_json(ctx, source[1], output.x) && from_json(ctx, source[2], output.y) && from_json(ctx, source[3], output.z);
            }
        };

        template <>
        struct type_traits<glm::quat> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, glm::quat& output)
            {
                if (source.isArray()) {
                    if (source.size() == 4) {
                        return from_json(ctx, source[0], output.w) && from_json(ctx, source[1], output.x) && from_json(ctx, source[2], output.y) && from_json(ctx, source[3], output.z);
                    } else if (source.size() == 3) {
                        glm::vec3 e;
                        if (from_json(ctx, source, e)) {
                            output = glm::quat{ glm::radians(e) };
                            return true;
                        }
                    }
                    return false;
                } else {
                    if (!source.isMember("x") || !source.isMember("y") || !source.isMember("z"))
                        return false;
                    if (source.isMember("w")) {
                        return from_json(ctx, source["w"], output.w) && from_json(ctx, source["x"], output.x) && from_json(ctx, source["y"], output.y) && from_json(ctx, source["z"], output.z);
                    } else {
                        glm::vec3 e;
                        if (from_json(ctx, source, e)) {
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
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, glm::mat2& output)
            {
                return source.isArray() && source.size() == 2 && from_json(ctx, source[0], output[0]) && from_json(ctx, source[1], output[1]);
            }
        };

        template <>
        struct type_traits<glm::mat3> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, glm::mat3& output)
            {
                return source.isArray() && source.size() == 3 && from_json(ctx, source[0], output[0]) && from_json(ctx, source[1], output[1]) && from_json(ctx, source[2], output[2]);
            }
        };

        template <>
        struct type_traits<glm::mat4> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, glm::mat4& output)
            {
                return source.isArray() && source.size() == 4 && from_json(ctx, source[0], output[0]) && from_json(ctx, source[1], output[1]) && from_json(ctx, source[2], output[2]) && from_json(ctx, source[3], output[3]);
            }
        };

        template <>
        struct type_traits<boost::filesystem::path> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, boost::filesystem::path& output)
            {
                output = source.asString();
                return true;
            }
        };

        template <class Resource>
        struct type_traits<resource::handle<Resource>> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, resource::handle<Resource>& output)
            {
                output = ctx.template get_cache<Resource>().handle_for(tools::resource_id_for({ source.asString() }));
                return false;
            }
        };

        template <>
        struct type_traits<graphics::technique_identifier> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, graphics::technique_identifier& output)
            {
                // TODO add any other other shaders here.

                // TODO warn/error about missing vertex shader.
                output.vertex = source["vertex"].asString();
                if (!boost::starts_with(output.vertex.string(), "vertex"))
                    output.vertex = "vertex" / output.vertex;

                if (source.isMember("tessellation_control")) {
                    output.tessellation_control = source["tessellation_control"].asString();
                    if (!boost::starts_with(output.tessellation_control.string(), "tessellation_control"))
                        output.tessellation_control = "tessellation_control" / output.tessellation_control;
                }
                if (source.isMember("tessellation_evaluation")) {
                    output.tessellation_evaluation = source["tessellation_evaluation"].asString();
                    if (!boost::starts_with(output.tessellation_evaluation.string(), "tessellation_evaluation"))
                        output.tessellation_evaluation = "tessellation_evaluation" / output.tessellation_evaluation;
                }
                if (source.isMember("geometry")) {
                    output.geometry = source["geometry"].asString();
                    if (!boost::starts_with(output.geometry.string(), "geometry"))
                        output.geometry = "geometry" / output.geometry;
                }

                // TODO warn/error about missing fragment shader.
                output.fragment = source["fragment"].asString();
                if (!boost::starts_with(output.fragment.string(), "fragment"))
                    output.fragment = "fragment" / output.fragment;

                return true;
            }
        };

        template <>
        struct type_traits<resource::handle<graphics::technique>> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, resource::handle<graphics::technique>& output)
            {
                graphics::technique_identifier tech_id;
                from_json(ctx, source, tech_id);

                // TODO add any other other shaders here.
                tools::complex_resource_id cid{
                    tech_id.vertex,
                    tech_id.tessellation_control,
                    tech_id.tessellation_evaluation,
                    tech_id.geometry,
                    tech_id.fragment
                };

                output = ctx.template get_cache<graphics::technique>().handle_for(tools::resource_id_for(cid));

                return true;
            }
        };

        template <class T, long int N>
        struct type_traits<T[N]> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, T* output)
            {
                for (long int i = 0; i < N; ++i) {
                    if (!from_json(ctx, source[(int)i], output[i]))
                        return false;
                }
                return true;
            }
        };

        template <class T>
        struct type_traits<std::vector<T>> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, std::vector<T>& output)
            {
                output.resize(source.size());
                for (std::size_t i = 0; i < output.size(); ++i) {
                    if (!from_json(ctx, source[(int)i], output[i]))
                        return false;
                }
                return true;
            }
        };

        template <>
        struct type_traits<sigma::graphics::texture_filter> {
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, sigma::graphics::texture_filter& output)
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
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, sigma::graphics::texture_format& output)
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
            template <class Context>
            static bool from(Context& ctx, const Json::Value& source, T& output)
            {
                // TODO check that the type actually has  keys
                bool good = true;
                boost::hana::for_each(boost::hana::keys(output), [&](auto key) {
                    auto& member = boost::hana::at_key(output, key);
                    using member_type = std::remove_reference_t<decltype(member)>;
                    if (source.isMember(key.c_str())) {
                        if (!type_traits<member_type>::from(ctx, source[key.c_str()], member))
                            good = false;
                    }
                });
                return true;
            }
        };
    }
}
}

#endif // SIGMA_ENGINE_JSON_CONVERSION_HPP

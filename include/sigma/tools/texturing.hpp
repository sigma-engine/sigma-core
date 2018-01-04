#ifndef SIGMA_TOOLS_TEXTURING_HPP
#define SIGMA_TOOLS_TEXTURING_HPP

#include <sigma/graphics/cubemap.hpp>

#include <boost/gil/image.hpp>

#include <glm/gtc/constants.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <cmath>

namespace sigma {
namespace tools {
    template <class Image, class Vec2>
    typename Image::value_type nearest_neighbor_sample(const Image& tex, const glm::vec2& uv)
    {
        return tex(std::round(tex.width() * uv.x), std::round(tex.height() * uv.y));
    }

    template <class Image>
    typename Image::value_type bilinear_sample(const Image& tex, const glm::vec2& uv)
    {
        float u = uv.x * tex.width() - 0.5;
        float v = uv.y * tex.height() - 0.5;

        int x = std::floor(u);
        int y = std::floor(v);

        float u_ratio = u - x;
        float v_ratio = v - y;
        float u_opposite = 1 - u_ratio;
        float v_opposite = 1 - v_ratio;

        int x_p1 = std::min(x + 1, int(tex.width()) - 1);
        int y_p1 = std::min(y + 1, int(tex.height()) - 1);
        typename Image::value_type output;
        for (int c = 0; c < boost::gil::num_channels<Image>::value; ++c)
            output[c] = (tex(x, y)[c] * u_opposite + tex(x_p1, y)[c] * u_ratio) * v_opposite + (tex(x, y_p1)[c] * u_opposite + tex(x_p1, y_p1)[c] * u_ratio) * v_ratio;
        return output;
    }

    glm::vec3 cubemap_normal(const graphics::cubemap::face& face, float clip_x, float clip_y)
    {
        switch (face) {
        case graphics::cubemap::face::POSITIVE_X: {
            return glm::normalize(glm::vec3{ 1.0f, clip_y, clip_x });
        }
        case graphics::cubemap::face::NEGATIVE_X: {
            return glm::normalize(glm::vec3{ -1.0f, clip_y, -clip_x });
        }
        case graphics::cubemap::face::POSITIVE_Y: {
            return glm::normalize(glm::vec3{ -clip_x, 1.0f, -clip_y });
        }
        case graphics::cubemap::face::NEGATIVE_Y: {
            return glm::normalize(glm::vec3{ -clip_x, -1.0f, clip_y });
        }
        case graphics::cubemap::face::POSITIVE_Z: {
            return glm::normalize(glm::vec3{ -clip_x, clip_y, 1.0f });
        }
        case graphics::cubemap::face::NEGATIVE_Z: {
            return glm::normalize(glm::vec3{ clip_x, clip_y, -1.0f });
        }
        }
        return {};
    }

    glm::vec2 equirectangular_mapping(const glm::vec3& N)
    {
        float x = 0.5f - std::atan2(N.x, N.z) / (2.0f * glm::pi<float>());
        float y = std::acos(N.y) / glm::pi<float>();
        return glm::vec2(x, y);
    }

    template <class SourceView, class DestView>
    void equirectangular_to_cubemap_face(const graphics::cubemap::face& face,
        const SourceView& source_view,
        DestView& output)
    {
        const int SIZE = output.width();
        for (int y = 0; y < output.height(); ++y) {
            float clip_y = -2 * (float(y) / SIZE) + 1;

            auto dst_it = output.row_begin(y);
            for (int x = 0; x < output.width(); ++x) {
                float clip_x = -2.0f * (float(x) / SIZE) + 1.0f;
                auto N = cubemap_normal(face, clip_x, clip_y);
                auto uv = equirectangular_mapping(N);
                dst_it[x] = bilinear_sample(source_view, uv);
            }
        }
    }
}
}

#endif // SIGMA_TOOLS_TEXTURING_HPP

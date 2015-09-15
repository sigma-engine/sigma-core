#ifndef SIGMAFIVE_GRAPHICS_OPENGL_TEXTURE2D_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_TEXTURE2D_HPP


#include <gl_core_4_0.hpp>

#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/vec4.hpp>

#include <vector>

namespace sigmafive {
    namespace graphics {
        namespace opengl {
            enum class texture_2d_target : GLenum {
                texture_2d = gl::TEXTURE_2D,
                proxy_texture_2d = gl::PROXY_TEXTURE_2D,
                texture_1d_array = gl::TEXTURE_1D_ARRAY,
                proxy_texture_1d_array = gl::PROXY_TEXTURE_1D_ARRAY,

                texture_rectangle = gl::TEXTURE_RECTANGLE,
                proxy_texture_rectangle = gl::PROXY_TEXTURE_RECTANGLE,

                //TODO when we have a cube map remove all of theses
                texture_cube_map_positive_x = gl::TEXTURE_CUBE_MAP_POSITIVE_X,
                texture_cube_map_negative_x = gl::TEXTURE_CUBE_MAP_NEGATIVE_X,
                texture_cube_map_positive_y = gl::TEXTURE_CUBE_MAP_POSITIVE_Y,
                texture_cube_map_negative_y = gl::TEXTURE_CUBE_MAP_NEGATIVE_Y,
                texture_cube_map_positive_z = gl::TEXTURE_CUBE_MAP_POSITIVE_Z,
                texture_cube_map_negative_z = gl::TEXTURE_CUBE_MAP_NEGATIVE_Z,
                proxy_texture_cube_map = gl::PROXY_TEXTURE_CUBE_MAP
            };

            enum class wrapping_type {
                repeat = gl::REPEAT,
                mirrored_repeat = gl::MIRRORED_REPEAT,
                clamp_to_edge = gl::CLAMP_TO_EDGE,
                clamp_to_border = gl::CLAMP_TO_BORDER
            };

            enum class filtering_mode {
                nearest = gl::NEAREST,
                linear = gl::LINEAR
            };

            enum class texture_format : GLenum {
                depth_component = gl::DEPTH_COMPONENT,
                depth_stencil = gl::DEPTH_STENCIL,
                red = gl::RED,
                green = gl::GREEN,
                rg = gl::RG,
                rgb = gl::RGB,
                rgba = gl::RGBA,

                //sized formats
                depth_component16 = gl::DEPTH_COMPONENT16,
                depth_component24 = gl::DEPTH_COMPONENT24,
                depth_component32 = gl::DEPTH_COMPONENT32,
                depth_component32f = gl::DEPTH_COMPONENT32F,
                depth24_stencil8 = gl::DEPTH24_STENCIL8,
                depth32F_stencil8 = gl::DEPTH32F_STENCIL8,
                r8 = gl::R8,
                r8_snorm = gl::R8_SNORM,
                r16 = gl::R16,
                r16_snorm = gl::R16_SNORM,
                rg8 = gl::RG8,
                rg8_snorm = gl::RG8_SNORM,
                rg16 = gl::RG16,
                rg16_snorm = gl::RG16_SNORM,
                r3_g3_b2 = gl::R3_G3_B2,
                rgb4 = gl::RGB4,
                rgb5 = gl::RGB5,
                rgb8 = gl::RGB8,
                rgb8_snorm = gl::RGB8_SNORM,
                rgb10 = gl::RGB10,
                rgb12 = gl::RGB12,
                rgb16_snorm = gl::RGB16_SNORM,
                rgba2 = gl::RGBA2,
                rgba4 = gl::RGBA4,
                rgb5_a1 = gl::RGB5_A1,
                rgba8 = gl::RGBA8,
                rgba8_snorm = gl::RGBA8_SNORM,
                rgb10_a2 = gl::RGB10_A2,
                rgb10_a2ui = gl::RGB10_A2UI,
                rgba12 = gl::RGBA12,
                rgba16 = gl::RGBA16,
                srgb8 = gl::SRGB8,
                srgb8_alpha8 = gl::SRGB8_ALPHA8,
                r16f = gl::R16F,
                rg16f = gl::RG16F,
                rgb16f = gl::RGB16F,
                rgba16f = gl::RGBA16F,
                r32f = gl::R32F,
                rg32f = gl::RG32F,
                rgb32f = gl::RGB32F,
                rgba32f = gl::RGBA32F,
                r11f_g11f_b10f = gl::R11F_G11F_B10F,
                rgb9_e5 = gl::RGB9_E5,
                r8i = gl::R8I,
                r8ui = gl::R8UI,
                r16i = gl::R16I,
                r16ui = gl::R16UI,
                r32i = gl::R32I,
                r32ui = gl::R32UI,
                rg8i = gl::RG8I,
                rg8ui = gl::RG8UI,
                rg16i = gl::RG16I,
                rg16ui = gl::RG16UI,
                rg32i = gl::RG32I,
                rg32ui = gl::RG32UI,
                rgb8i = gl::RGB8I,
                rgb8ui = gl::RGB8UI,
                rgb16i = gl::RGB16I,
                rgb16ui = gl::RGB16UI,
                rgb32i = gl::RGB32I,
                rgb32ui = gl::RGB32UI,
                rgba8i = gl::RGBA8I,
                rgba8ui = gl::RGBA8UI,
                rgba16i = gl::RGBA16I,
                rgba16ui = gl::RGBA16UI,
                rgba32i = gl::RGBA32I,
                rgba32ui = gl::RGBA32UI,

                //compressed formats
                compressed_red = gl::COMPRESSED_RED,
                compressed_rg = gl::COMPRESSED_RG,
                compressed_rgb = gl::COMPRESSED_RGB,
                compressed_rgba = gl::COMPRESSED_RGBA,
                compressed_srgb = gl::COMPRESSED_SRGB,
                compressed_srgb_alpha = gl::COMPRESSED_SRGB_ALPHA,
                compressed_red_rgtc1 = gl::COMPRESSED_RED_RGTC1,
                compressed_signed_red_rgtc1 = gl::COMPRESSED_SIGNED_RED_RGTC1,
                compressed_rg_rgtc2 = gl::COMPRESSED_RG_RGTC2,
                compressed_signed_rg_rgtc2 = gl::COMPRESSED_SIGNED_RG_RGTC2,
                //compressed_rgba_bptc_unorm = gl::COMPRESSED_RGBA_BPTC_UNORM,
                //compressed_srgb_alpha_bptc_unorm = gl::COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
                //compressed_rgb_bptc_signed_float = gl::COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
                //compressed_rgb_bptc_unsigned_float = gl::COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
            };

            enum class pixel_format : GLenum  {
                red = gl::RED,
                rg = gl::RG,
                rgb = gl::RGB,
                bgr = gl::BGR,
                rgba = gl::RGBA,
                bgra = gl::BGRA,
                red_integer = gl::RED_INTEGER,
                rg_integer = gl::RG_INTEGER,
                rgb_integer = gl::RGB_INTEGER,
                bgr_integer = gl::BGR_INTEGER,
                rgba_integer = gl::RGBA_INTEGER,
                bgra_integer = gl::BGRA_INTEGER,
                stencil_index = gl::STENCIL_INDEX,
                depth_component = gl::DEPTH_COMPONENT,
                depth_stencil = gl::DEPTH_STENCIL,
            };

            enum class pixel_type : GLenum  {
                unsigned_byte = gl::UNSIGNED_BYTE,
                byte = gl::BYTE,
                unsigned_short = gl::UNSIGNED_SHORT,
                short_ = gl::SHORT,
                unsigned_int = gl::UNSIGNED_INT,
                int_ = gl::INT,
                float_ = gl::FLOAT,
                unsigned_byte_3_3_2 = gl::UNSIGNED_BYTE_3_3_2,
                unsigned_byte_2_3_3_rev = gl::UNSIGNED_BYTE_2_3_3_REV,
                unsigned_short_5_6_5 = gl::UNSIGNED_SHORT_5_6_5,
                unsigned_short_5_6_5_rev = gl::UNSIGNED_SHORT_5_6_5_REV,
                unsigned_short_4_4_4_4 = gl::UNSIGNED_SHORT_4_4_4_4,
                unsigned_short_4_4_4_4_rev = gl::UNSIGNED_SHORT_4_4_4_4_REV,
                unsigned_short_5_5_5_1 = gl::UNSIGNED_SHORT_5_5_5_1,
                unsigned_short_1_5_5_5_rev = gl::UNSIGNED_SHORT_1_5_5_5_REV,
                unsigned_int_8_8_8_8 = gl::UNSIGNED_INT_8_8_8_8,
                unsigned_int_8_8_8_8_rev = gl::UNSIGNED_INT_8_8_8_8_REV,
                unsigned_int_10_10_10_2 = gl::UNSIGNED_INT_10_10_10_2,
                unsigned_int_2_10_10_10_rev = gl::UNSIGNED_INT_2_10_10_10_REV
            };

            class texture_2d {
            public:
                texture_2d();

                texture_2d(texture_format format, uint2 size,pixel_format px_format,pixel_type px_type);

                ~texture_2d();

                operator GLuint() const;

                void bind();

                void set_pixels(int2 size, const std::vector<vec4_t<unsigned char>> &pixels);

            private:
                GLuint gl_object_;
            };
        }
    }
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_TEXTURE2D_HPP

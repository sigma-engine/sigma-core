#ifndef SIGMA_ENGINE_OPENGL_TEXTURE2D_HPP
#define SIGMA_ENGINE_OPENGL_TEXTURE2D_HPP

#include <sigma/opengl/gl_core_4_0.h>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <vector>

namespace sigma {
namespace opengl {
    /*enum class texture_2d_target : GLenum {
    texture_2d = GL_TEXTURE_2D,
    proxy_texture_2d = GL_PROXY_TEXTURE_2D,
    texture_1d_array = GL_TEXTURE_1D_ARRAY,
    proxy_texture_1d_array = GL_PROXY_TEXTURE_1D_ARRAY,

    texture_rectangle = GL_TEXTURE_RECTANGLE,
    proxy_texture_rectangle = GL_PROXY_TEXTURE_RECTANGLE,

    //TODO when we have a cube map remove all of theses
    texture_cube_map_positive_x = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
    texture_cube_map_negative_x = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
    texture_cube_map_positive_y = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
    texture_cube_map_negative_y = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
    texture_cube_map_positive_z = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
    texture_cube_map_negative_z = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    proxy_texture_cube_map = GL_PROXY_TEXTURE_CUBE_MAP
};

enum class wrapping_type {
    repeat = GL_REPEAT,
    mirrored_repeat = GL_MIRRORED_REPEAT,
    clamp_to_edge = GL_CLAMP_TO_EDGE,
    clamp_to_border = GL_CLAMP_TO_BORDER
};

enum class filtering_mode {
    nearest = GL_NEAREST,
    linear = GL_LINEAR
};

enum class texture_format : GLenum {
    depth_component = GL_DEPTH_COMPONENT,
    depth_stencil = GL_DEPTH_STENCIL,
    red = GL_RED,
    green = GL_GREEN,
    rg = GL_RG,
    rgb = GL_RGB,
    rgba = GL_RGBA,

    //sized formats
    depth_component16 = GL_DEPTH_COMPONENT16,
    depth_component24 = GL_DEPTH_COMPONENT24,
    depth_component32 = GL_DEPTH_COMPONENT32,
    depth_component32f = GL_DEPTH_COMPONENT32F,
    depth24_stencil8 = GL_DEPTH24_STENCIL8,
    depth32F_stencil8 = GL_DEPTH32F_STENCIL8,
    r8 = GL_R8,
    r8_snorm = GL_R8_SNORM,
    r16 = GL_R16,
    r16_snorm = GL_R16_SNORM,
    rg8 = GL_RG8,
    rg8_snorm = GL_RG8_SNORM,
    rg16 = GL_RG16,
    rg16_snorm = GL_RG16_SNORM,
    r3_g3_b2 = GL_R3_G3_B2,
    rgb4 = GL_RGB4,
    rgb5 = GL_RGB5,
    rgb8 = GL_RGB8,
    rgb8_snorm = GL_RGB8_SNORM,
    rgb10 = GL_RGB10,
    rgb12 = GL_RGB12,
    rgb16_snorm = GL_RGB16_SNORM,
    rgba2 = GL_RGBA2,
    rgba4 = GL_RGBA4,
    rgb5_a1 = GL_RGB5_A1,
    rgba8 = GL_RGBA8,
    rgba8_snorm = GL_RGBA8_SNORM,
    rgb10_a2 = GL_RGB10_A2,
    rgb10_a2ui = GL_RGB10_A2UI,
    rgba12 = GL_RGBA12,
    rgba16 = GL_RGBA16,
    srgb8 = GL_SRGB8,
    srgb8_alpha8 = GL_SRGB8_ALPHA8,
    r16f = GL_R16F,
    rg16f = GL_RG16F,
    rgb16f = GL_RGB16F,
    rgba16f = GL_RGBA16F,
    r32f = GL_R32F,
    rg32f = GL_RG32F,
    rgb32f = GL_RGB32F,
    rgba32f = GL_RGBA32F,
    r11f_g11f_b10f = GL_R11F_G11F_B10F,
    rgb9_e5 = GL_RGB9_E5,
    r8i = GL_R8I,
    r8ui = GL_R8UI,
    r16i = GL_R16I,
    r16ui = GL_R16UI,
    r32i = GL_R32I,
    r32ui = GL_R32UI,
    rg8i = GL_RG8I,
    rg8ui = GL_RG8UI,
    rg16i = GL_RG16I,
    rg16ui = GL_RG16UI,
    rg32i = GL_RG32I,
    rg32ui = GL_RG32UI,
    rgb8i = GL_RGB8I,
    rgb8ui = GL_RGB8UI,
    rgb16i = GL_RGB16I,
    rgb16ui = GL_RGB16UI,
    rgb32i = GL_RGB32I,
    rgb32ui = GL_RGB32UI,
    rgba8i = GL_RGBA8I,
    rgba8ui = GL_RGBA8UI,
    rgba16i = GL_RGBA16I,
    rgba16ui = GL_RGBA16UI,
    rgba32i = GL_RGBA32I,
    rgba32ui = GL_RGBA32UI,

    //compressed formats
    compressed_red = GL_COMPRESSED_RED,
    compressed_rg = GL_COMPRESSED_RG,
    compressed_rgb = GL_COMPRESSED_RGB,
    compressed_rgba = GL_COMPRESSED_RGBA,
    compressed_srgb = GL_COMPRESSED_SRGB,
    compressed_srgb_alpha = GL_COMPRESSED_SRGB_ALPHA,
    compressed_red_rgtc1 = GL_COMPRESSED_RED_RGTC1,
    compressed_signed_red_rgtc1 = GL_COMPRESSED_SIGNED_RED_RGTC1,
    compressed_rg_rgtc2 = GL_COMPRESSED_RG_RGTC2,
    compressed_signed_rg_rgtc2 = GL_COMPRESSED_SIGNED_RG_RGTC2,
    //compressed_rgba_bptc_unorm = GL_COMPRESSED_RGBA_BPTC_UNORM,
    //compressed_srgb_alpha_bptc_unorm = GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM,
    //compressed_rgb_bptc_signed_float = GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT,
    //compressed_rgb_bptc_unsigned_float =
GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT,
};

enum class pixel_format : GLenum {
    red = GL_RED,
    rg = GL_RG,
    rgb = GL_RGB,
    bgr = GL_BGR,
    rgba = GL_RGBA,
    bgra = GL_BGRA,
    red_integer = GL_RED_INTEGER,
    rg_integer = GL_RG_INTEGER,
    rgb_integer = GL_RGB_INTEGER,
    bgr_integer = GL_BGR_INTEGER,
    rgba_integer = GL_RGBA_INTEGER,
    bgra_integer = GL_BGRA_INTEGER,
    stencil_index = GL_STENCIL_INDEX,
    depth_component = GL_DEPTH_COMPONENT,
    depth_stencil = GL_DEPTH_STENCIL,
};

enum class pixel_type : GLenum {
    unsigned_byte = GL_UNSIGNED_BYTE,
    byte = GL_BYTE,
    unsigned_short = GL_UNSIGNED_SHORT,
    short_ = GL_SHORT,
    unsigned_int = GL_UNSIGNED_INT,
    int_ = GL_INT,
    float_ = GL_FLOAT,
    unsigned_byte_3_3_2 = GL_UNSIGNED_BYTE_3_3_2,
    unsigned_byte_2_3_3_rev = GL_UNSIGNED_BYTE_2_3_3_REV,
    unsigned_short_5_6_5 = GL_UNSIGNED_SHORT_5_6_5,
    unsigned_short_5_6_5_rev = GL_UNSIGNED_SHORT_5_6_5_REV,
    unsigned_short_4_4_4_4 = GL_UNSIGNED_SHORT_4_4_4_4,
    unsigned_short_4_4_4_4_rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
    unsigned_short_5_5_5_1 = GL_UNSIGNED_SHORT_5_5_5_1,
    unsigned_short_1_5_5_5_rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
    unsigned_int_8_8_8_8 = GL_UNSIGNED_INT_8_8_8_8,
    unsigned_int_8_8_8_8_rev = GL_UNSIGNED_INT_8_8_8_8_REV,
    unsigned_int_10_10_10_2 = GL_UNSIGNED_INT_10_10_10_2,
    unsigned_int_2_10_10_10_rev = GL_UNSIGNED_INT_2_10_10_10_REV
};

class SIGMA_ENGINE_OPENGL_API texture_2d {
public:
    texture_2d();

    texture_2d(texture_format format, glm::ivec2 size, pixel_format px_format,
pixel_type px_type);

    ~texture_2d();

    operator GLuint() const;

    void bind(unsigned int texture_unit);

    void set_pixels(glm::ivec2 size, const unsigned char* pixels); //const
std::vector<vec4_t<unsigned char>> &pixels

private:
    GLuint gl_object_;
};*/
}
}

#endif // SIGMA_ENGINE_OPENGL_TEXTURE2D_HPP

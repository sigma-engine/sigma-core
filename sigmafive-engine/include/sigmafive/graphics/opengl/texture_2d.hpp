#ifndef SIGMAFIVE_GRAPHICS_OPENGL_TEXTURE2D_HPP
#define SIGMAFIVE_GRAPHICS_OPENGL_TEXTURE2D_HPP

#include <sigmafive/math/vec2.hpp>
#include <sigmafive/math/vec4.hpp>
#include <sigmafive/graphics/opengl/gl_core_4_0.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
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

			class texture_2d {
			public:
				texture_2d(wrapping_type u_wrapping=wrapping_type::repeat,
						  wrapping_type v_wrapping=wrapping_type::repeat,
						  float4 border_color = float4(0,0,0,1),
						  filtering_mode minification_filtering_mode=filtering_mode::nearest,
						  filtering_mode magnification_filtering_mode=filtering_mode::nearest);

				~texture_2d();

				wrapping_type u_wrapping() const;

				void set_u_wrapping(wrapping_type u_wrapping);

				wrapping_type v_wrapping() const;

				void set_v_wrapping(wrapping_type v_wrapping);

				filtering_mode minification_filtering_mode() const;

				void set_minification_filtering_mode(filtering_mode minification_filtering_mode);

				filtering_mode magnification_filtering_mode() const;

				void set_magnification_filtering_mode(filtering_mode magnification_filtering_mode);

				float4 border_color() const;

				void set_border_color(float4 border_color);

				void bind() const;

				int2 size() const;

				void set_pixels(int2 size,const std::vector<vec4_t<unsigned char>> &pixels);
			private:
				GLuint gl_object_;
			};
		}
	}
}

#endif //SIGMAFIVE_GRAPHICS_OPENGL_TEXTURE2D_HPP

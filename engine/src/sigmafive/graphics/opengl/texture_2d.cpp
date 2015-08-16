#include <graphics/opengl/texture_2d.hpp>
#include <gl_core_4_0.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			//TODO add Mipmaping support

			texture_2d::texture_2d(wrapping_type u_wrapping,
								 wrapping_type v_wrapping,
					             float4 border_color,
								 filtering_mode minification_filtering_mode,
								 filtering_mode magnification_filtering_mode) {
				gl::GenTextures(1,&gl_object_); //TODO GL_CHECK_ERROR;

				//setup the wrapping modes
				set_u_wrapping(u_wrapping);
				set_v_wrapping(v_wrapping);

				//setup the border color
				set_border_color(border_color);

				//setup the filtering modes
				set_minification_filtering_mode(minification_filtering_mode);
				set_magnification_filtering_mode(magnification_filtering_mode);
			}

			texture_2d::~texture_2d() {
				gl::DeleteTextures(1, &gl_object_); //TODO GL_CHECK_ERROR;
			}

			wrapping_type texture_2d::u_wrapping() const {
				//TODO this changes global state
				bind();

				GLint wrapping;
				gl::GetTexParameteriv(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, &wrapping); //TODO GL_CHECK_ERROR;
				return (wrapping_type)wrapping;
			}

			void texture_2d::set_u_wrapping(wrapping_type u_wrapping) {
				//TODO this changes global state
				bind();

				gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_S, (GLenum)u_wrapping); //TODO GL_CHECK_ERROR;
			}

			wrapping_type texture_2d::v_wrapping() const {
				//TODO this changes global state
				bind();

				GLint wrapping;
				gl::GetTexParameteriv(gl::TEXTURE_2D,gl::TEXTURE_WRAP_T,&wrapping); //TODO GL_CHECK_ERROR;
				return (wrapping_type)wrapping;
			}

			void texture_2d::set_v_wrapping(wrapping_type v_wrapping) {
				//TODO this changes global state
				bind();

				gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_WRAP_T, (GLenum)v_wrapping); //TODO GL_CHECK_ERROR;
			}

			filtering_mode texture_2d::minification_filtering_mode() const {
				//TODO this changes global state
				bind();

				GLint filter;
				gl::GetTexParameteriv(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, &filter); //TODO GL_CHECK_ERROR;
				return (filtering_mode)filter;
			}

			void texture_2d::set_minification_filtering_mode(filtering_mode minification_filtering_mode) {
				//TODO this changes global state
				bind();

				gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, (GLenum)minification_filtering_mode); //TODO GL_CHECK_ERROR;
			}

			filtering_mode texture_2d::magnification_filtering_mode() const {
				//TODO this changes global state
				bind();

				GLint filter;
				gl::GetTexParameteriv(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, &filter); //TODO GL_CHECK_ERROR;
				return (filtering_mode)filter;
			}

			void texture_2d::set_magnification_filtering_mode(filtering_mode magnification_filtering_mode) {
				//TODO this changes global state
				bind();

				gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, (GLenum)magnification_filtering_mode); //TODO GL_CHECK_ERROR;
			}

			float4 texture_2d::border_color() const {
				//TODO this changes global state
				bind();

				float4 border_color;
				gl::GetTexParameterfv(gl::TEXTURE_2D, gl::TEXTURE_BORDER_COLOR, &border_color[0]); //TODO GL_CHECK_ERROR; and border_color.data()
				return border_color;
			}

			void texture_2d::set_border_color(float4 border_color) {
				//TODO this changes global state
				bind();

				gl::TexParameterfv(gl::TEXTURE_2D, gl::TEXTURE_BORDER_COLOR, &border_color[0]); //TODO GL_CHECK_ERROR; and border_color.data()
			}

			void texture_2d::bind() const {
				gl::BindTexture(gl::TEXTURE_2D,gl_object_); //TODO GL_CHECK_ERROR;
			}

			void texture_2d::set_pixels(int2 size,const std::vector<vec4_t<unsigned char>> &pixels) {
				//TODO this changes global state
				bind();

				gl::TexImage2D(gl::TEXTURE_2D,0,gl::RGBA,size.x,size.y,0,gl::RGBA,gl::UNSIGNED_BYTE,pixels.data());
			}

			int2 texture_2d::size() const {
				//TODO this changes global state
				bind();

				int2 size;
				gl::GetTexParameteriv(gl::TEXTURE_2D, gl::TEXTURE_WIDTH, &size.x); //TODO GL_CHECK_ERROR;
				gl::GetTexParameteriv(gl::TEXTURE_2D, gl::TEXTURE_HEIGHT, &size.y); //TODO GL_CHECK_ERROR;

				return size;
			}
		}
	}
}

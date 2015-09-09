#include <shader.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			shader::shader(shader_type type) :
				type_(type),gl_object_(0),compiled_(false) {
				gl_object_ = gl::CreateShader((GLenum)type_); //TODO GL_CHECK_ERROR;
			}

			shader::~shader() {
				gl::DeleteShader(gl_object_); //TODO GL_CHECK_ERROR;
			}

			bool shader::operator==(const shader &other) const {
				return gl_object_ == other.gl_object_ && type_ == other.type_; //TODO compiled???
			}

			shader_type shader::type() const {
				return type_;
			}

			bool shader::is_compiled() const {
				return compiled_;
			}

			void shader::set_source(std::string source) {
				const char *c_str = source.c_str();
				gl::ShaderSource(gl_object_,1,&c_str,nullptr); //TODO GL_CHECK_ERROR;
				compiled_ = false;
			}

			std::string shader::compile() {
				if(!compiled_) {
					gl::CompileShader(gl_object_); //TODO GL_CHECK_ERROR;

					GLint success = 0;
					gl::GetShaderiv(gl_object_, gl::COMPILE_STATUS, &success); //TODO GL_CHECK_ERROR;
					compiled_ = (success == gl::TRUE_);

					if (!compiled_) {
						GLint log_length = 0;
						gl::GetShaderiv(gl_object_, gl::INFO_LOG_LENGTH, &log_length); //TODO GL_CHECK_ERROR;

						std::string message;
						message.resize(static_cast<std::size_t>(log_length));

						gl::GetShaderInfoLog(gl_object_, log_length, &log_length, (GLchar*)message.data()); //TODO GL_CHECK_ERROR;
						return message;
					}
				}
				return "";
			}

            shader::operator GLuint() const {
                return gl_object_;
            }
        }
	}
}

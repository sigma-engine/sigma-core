#include <graphics/opengl/program.hpp>
#include <algorithm>
#include <gl_core_4_0.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			program::program() :
				gl_object_(2),linked_(false) {
				gl_object_ = gl::CreateProgram(); //TODO GL_CHECK_ERROR;
			}

			program::~program() {
				gl::DeleteProgram(gl_object_); //TODO GL_CHECK_ERROR;
			}

			bool program::is_linked() const {
				return linked_ && std::all_of(begin(attached_shaders_),end(attached_shaders_),
						                      std::function<bool(const shader &)>(&shader::is_compiled));
			}

			void program::attach(shader &s) {
				attached_shaders_.emplace_back(s);
				linked_ = false;
			}

			void program::detach(shader &s) {
				attached_shaders_.erase(std::remove_if(begin(attached_shaders_),end(attached_shaders_),
								                       std::bind(std::equal_to<const shader &>(),std::placeholders::_1,s)),
						                attached_shaders_.end());
				linked_ = false;
			}

			std::string program::link() {
				if(!is_linked()) {
					//compile and accumulate the shader error messages
					auto message = std::accumulate(begin(attached_shaders_),end(attached_shaders_),std::string(""),
                    [](std::string message,shader &s) {
						return message +
								(message.size() > 0 ? "\n" : "") +
								(s.is_compiled()? "": s.compile()); //TODO some how get filepath
					});
					if(message.size() > 0)
						return message;

					//attach the shaders to the program object
					std::for_each(begin(attached_shaders_),end(attached_shaders_),
					[=](shader &s) {
						gl::AttachShader(gl_object_,s); //TODO GL_CHECK_ERROR;
					});

					gl::LinkProgram(gl_object_); //TODO GL_CHECK_ERROR;
					GLint linked;
					gl::GetProgramiv(gl_object_,gl::LINK_STATUS,&linked); //TODO GL_CHECK_ERROR;
					linked_ = (linked==gl::TRUE_);

					//detach the shaders from the program object
					std::for_each(begin(attached_shaders_),end(attached_shaders_),
					[=](shader &s) {
						gl::DetachShader(gl_object_,s); //TODO GL_CHECK_ERROR;
					});

					if(!linked_) {
						int log_length = 0;
						gl::GetProgramiv(linked_,gl::INFO_LOG_LENGTH,&log_length); //TODO GL_CHECK_ERROR;

						std::string message;
						message.resize(log_length);

						gl::GetProgramInfoLog(gl_object_,log_length,&log_length,&message[0]); //TODO GL_CHECK_ERROR;

						return message;
					}
				}
				return "";
			}
		}
	}
}

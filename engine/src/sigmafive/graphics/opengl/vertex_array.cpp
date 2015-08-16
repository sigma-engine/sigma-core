#include <sigmafive/graphics/opengl/vertex_array.hpp>

namespace sigmafive {
	namespace graphics {
		namespace opengl {
			vertex_array::vertex_array()
				:gl_object_(0) {
				gl::GenVertexArrays(1,&gl_object_);//TODO GL_CHECK_ERROR;
			}

			vertex_array::~vertex_array() {
				gl::DeleteVertexArrays(1,&gl_object_);//TODO GL_CHECK_ERROR;don't throw in the destructor
			}

			void vertex_array::bind(const vertex_buffer_base &buffer, unsigned int index, unsigned int number_of_components, data_types component_type, bool normalize, const void *offset) {
				gl::BindVertexArray(gl_object_); //TODO GL_CHECK_ERROR;
				gl::BindBuffer(gl::ARRAY_BUFFER,buffer); //TODO GL_CHECK_ERROR;

				gl::EnableVertexAttribArray(index); //TODO GL_CHECK_ERROR; //TODO does this change gloabl state
				gl::VertexAttribPointer(index,number_of_components,component_type,static_cast<GLboolean>(normalize), static_cast<GLsizei>(buffer.stride()),offset); //TODO GL_CHECK_ERROR;
			}

			void vertex_array::draw(primitive_type mode, std::size_t first, std::size_t count) const {
				//TODO preserve opengl vertex array state??? or use direct state if is is available.
				gl::BindVertexArray(gl_object_); //TODO GL_CHECK_ERROR;
				gl::DrawArrays(static_cast<GLenum>(mode),first,count); //TODO GL_CHECK_ERROR;
			}

			void vertex_array::draw(primitive_type mode, const index_buffer &index_buffer, std::size_t first, std::size_t count) const {
				gl::BindVertexArray(gl_object_); //TODO GL_CHECK_ERROR;

				gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, index_buffer); //TODO index_buffer.bind()
				gl::DrawElements(
						gl::TRIANGLES,      // mode
						count,    // count
						gl::UNSIGNED_INT,   // type
						(void*)first           // element array buffer offset
				);
			}
		}
	}
}

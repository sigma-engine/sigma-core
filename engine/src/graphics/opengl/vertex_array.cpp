#include <graphics/opengl/vertex_array.hpp>
#include <gl_core_4_0.hpp>

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
		}
	}
}

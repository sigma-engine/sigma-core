#include <sigmafive/opengl/texture_2d.hpp>

namespace sigmafive {
namespace opengl {
    /*//TODO add mipmapping support
texture_2d::texture_2d()
{
    glGenTextures(1, &gl_object_); //TODO GL_CHECK_ERROR;
    //TODO this changes global state
    bind(0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

texture_2d::texture_2d(texture_format format, glm::ivec2 size, pixel_format
px_format, pixel_type px_type)
    : texture_2d()
{
    assert((size.x != 0 && size.y != 0) && "Can not allocate a 2D texture with a
size of zero. did you mean to use a 1D texture?");
    //TODO would be best if use glTextureStorage2D from 4.5
    glTexImage2D(GL_TEXTURE_2D, 0, GLenum(format), size.x, size.y, 0,
GLenum(px_format), GLenum(px_type), nullptr);
}

texture_2d::~texture_2d()
{
    glDeleteTextures(1, &gl_object_); //TODO GL_CHECK_ERROR;
}

texture_2d::operator GLuint() const
{
    return gl_object_;
}

void texture_2d::bind(unsigned int texture_unit)
{
    glActiveTexture(GL_TEXTURE0 + texture_unit);
    glBindTexture(GL_TEXTURE_2D, gl_object_); //TODO GL_CHECK_ERROR;
}

void texture_2d::set_pixels(glm::ivec2 size, const unsigned char* pixels)
{ //const std::vector<vec4_t<unsigned char>> &pixels
    //TODO this changes global state
    bind(0);
    //TODO would be best if use glTextureStorage2D from 4.5
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.x, size.y, 0, GL_RGBA,
GL_UNSIGNED_BYTE, pixels);
}*/
}
}

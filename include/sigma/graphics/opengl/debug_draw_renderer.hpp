#ifndef SIGMA_GRAPHICS_OPENGL_DEBUG_DRAW_RENDER_HPP
#define SIGMA_GRAPHICS_OPENGL_DEBUG_DRAW_RENDER_HPP

#include <sigma/config.hpp>

#include "debug_draw.hpp"

#include <glad/glad.h>

#include <glm/mat4x4.hpp>

namespace sigma {
namespace opengl {
    class debug_draw_renderer : public dd::RenderInterface {
    public:
        debug_draw_renderer();

        ~debug_draw_renderer();

        void drawPointList(const dd::DrawVertex* points, int count, bool depthEnabled) override;

        void drawLineList(const dd::DrawVertex* lines, int count, bool depthEnabled) override;

        void drawGlyphList(const dd::DrawVertex* glyphs, int count, dd::GlyphTextureHandle glyphTex) override;

        dd::GlyphTextureHandle createGlyphTexture(int width, int height, const void* pixels) override;

        void destroyGlyphTexture(dd::GlyphTextureHandle glyphTex) override;

        glm::mat4 mvpMatrix;
        int windowWidth;
        int windowHeight;

    private:
        void setupShaderPrograms();

        void setupVertexBuffers();

        GLuint handleToGL(dd::GlyphTextureHandle handle);

        dd::GlyphTextureHandle GLToHandle(const GLuint id);

        void checkGLError(const char* file, const int line);

        void compileShader(const GLuint shader);

        void linkProgram(const GLuint program);

        GLuint linePointProgram;
        GLint linePointProgram_MvpMatrixLocation;

        GLuint textProgram;
        GLint textProgram_GlyphTextureLocation;
        GLint textProgram_ScreenDimensions;

        GLuint linePointVAO;
        GLuint linePointVBO;

        GLuint textVAO;
        GLuint textVBO;
    };
}
}

#endif // SIGMA_GRAPHICS_OPENGL_DEBUG_DRAW_RENDER_HPP

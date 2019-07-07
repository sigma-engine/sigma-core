#include <sigma/OpenGL/DeviceManagerGL.hpp>

#include <sigma/Log.hpp>
#include <sigma/OpenGL/DeviceGL.hpp>
#include <sigma/OpenGL/UtilGL.hpp>

#include <glad/glad.h>

void glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
    switch (severity) {
    case GL_DEBUG_SEVERITY_LOW:
        SIGMA_WARN("0x{:x}: {}", id, message);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        SIGMA_ERROR("0x{:x}: {}", id, message);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        SIGMA_CRITICAL("0x{:x}: {}", id, message);
        break;
    default:
        return;
    }
}

DeviceManagerGL::DeviceManagerGL()
    : mGraphicsDevice(std::make_shared<DeviceGL>())
{
}

bool DeviceManagerGL::initialize(const std::set<std::string>& inRequiredExtensions)
{
    CHECK_GL(gladLoadGL());

    CHECK_GL(glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS));
    CHECK_GL(glDebugMessageCallback(glDebugCallback, nullptr));
    CHECK_GL(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, true));
    CHECK_GL(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr, true));
    CHECK_GL(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, true));
    CHECK_GL(glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, false));
    CHECK_GL(glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_TYPE_OTHER, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_FALSE));

    GLint extensionCount;
    CHECK_GL(glGetIntegerv(GL_NUM_EXTENSIONS, &extensionCount));
    for (GLint i = 0; i < extensionCount; ++i) {
        const char* ext = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, static_cast<GLuint>(i)));
        mExtensions.insert(ext);
    }

    bool hasMissingExtensions = false;
    for (auto reqExt : inRequiredExtensions) {
        if (!mExtensions.count(reqExt)) {
            hasMissingExtensions = true;
            SIGMA_ERROR("Missing Required OpenGL Extension: {}", reqExt);
        }
    }

    if (hasMissingExtensions) {
        SIGMA_INFO("Supported OpenGL Extensions: {}", fmt::join(mExtensions.begin(), mExtensions.end(), ","));
    }

    return true;
}

void DeviceManagerGL::enumerateSurfaceDevices(std::shared_ptr<Surface> inSurface, std::vector<std::shared_ptr<Device>>& outDevices)
{
    (void)inSurface;
    outDevices.clear();
    outDevices.push_back(mGraphicsDevice);
}

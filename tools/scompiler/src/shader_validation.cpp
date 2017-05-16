#include <shader_validation.hpp>

#include <sigma/graphics/shader.hpp>

#include <glslang/Public/ShaderLang.h>

#include <regex>

namespace glslang {
extern const TBuiltInResource DefaultTBuiltInResource;
}

struct glsl_process {
    glsl_process()
    {
        glslang::InitializeProcess();
    }
    ~glsl_process()
    {
        glslang::FinalizeProcess();
    }
};

bool sigma::validate_shader(const graphics::shader_data& shader, std::vector<boost::filesystem::path> source_filenames)
{
    glsl_process validation_process;

    EShLanguage glslLangType;
    switch (shader.type) {
    case graphics::shader_type::vertex: {
        glslLangType = EShLangVertex;
        break;
    }
    case graphics::shader_type::fragment: {
        glslLangType = EShLangFragment;
        break;
    }
    case graphics::shader_type::geometry: {
        glslLangType = EShLangGeometry;
        break;
    }
    case graphics::shader_type::header: {
        return false;
        break;
    }
    }

    glslang::TShader glslShader{ glslLangType };
    const char* src = shader.source.c_str();
    glslShader.setStrings(&src, 1);

    if (!glslShader.parse(&glslang::DefaultTBuiltInResource, 330, true, EShMsgDefault)) {

        std::string error_message = glslShader.getInfoLog();

        std::regex error_regex("error\\s*:\\s*(\\d+):(\\d+):(.*)", std::regex_constants::icase);
        std::smatch res;

        const auto begin_iterator = std::sregex_iterator(error_message.cbegin(), error_message.cend(), error_regex);
        const auto end_iterator = std::sregex_iterator();

        for (auto it = begin_iterator; it != end_iterator; ++it) {
            auto match = *it;
            std::cerr << source_filenames[std::stoi(match[1])].string() << "(" << match[2] << "): error :" << match[3] << '\n'; // MSVC output
        }

        return false;
    }

    return true;
}

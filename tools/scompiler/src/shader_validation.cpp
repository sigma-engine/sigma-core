#include <shader_validation.hpp>

#include <sigma/graphics/shader.hpp>

#ifdef USE_GLSLANG
#include <glslang/Public/ShaderLang.h>
#include <regex>

static const TBuiltInResource default_resources = {
    32, // MaxLights
    6, // MaxClipPlanes
    32, // MaxTextureUnits
    32, // MaxTextureCoords
    64, // MaxVertexAttribs
    4096, // MaxVertexUniformComponents
    64, // MaxVaryingFloats
    32, // MaxVertexTextureImageUnits
    80, // MaxCombinedTextureImageUnits
    32, // MaxTextureImageUnits
    4096, // MaxFragmentUniformComponents
    32, // MaxDrawBuffers
    128, // MaxVertexUniformVectors
    8, // MaxVaryingVectors
    16, // MaxFragmentUniformVectors
    16, // MaxVertexOutputVectors
    15, // MaxFragmentInputVectors
    -8, // MinProgramTexelOffset
    7, // MaxProgramTexelOffset
    8, // MaxClipDistances
    65535, // MaxComputeWorkGroupCountX
    65535, // MaxComputeWorkGroupCountY
    65535, // MaxComputeWorkGroupCountZ
    1024, // MaxComputeWorkGroupSizeX
    1024, // MaxComputeWorkGroupSizeY
    64, // MaxComputeWorkGroupSizeZ
    1024, // MaxComputeUniformComponents
    16, // MaxComputeTextureImageUnits
    8, // MaxComputeImageUniforms
    8, // MaxComputeAtomicCounters
    1, // MaxComputeAtomicCounterBuffers
    60, // MaxVaryingComponents
    64, // MaxVertexOutputComponents
    64, // MaxGeometryInputComponents
    128, // MaxGeometryOutputComponents
    128, // MaxFragmentInputComponents
    8, // MaxImageUnits
    8, // MaxCombinedImageUnitsAndFragmentOutputs
    8, // MaxCombinedShaderOutputResources
    0, // MaxImageSamples
    0, // MaxVertexImageUniforms
    0, // MaxTessControlImageUniforms
    0, // MaxTessEvaluationImageUniforms
    0, // MaxGeometryImageUniforms
    8, // MaxFragmentImageUniforms
    8, // MaxCombinedImageUniforms
    16, // MaxGeometryTextureImageUnits
    256, // MaxGeometryOutputVertices
    1024, // MaxGeometryTotalOutputComponents
    1024, // MaxGeometryUniformComponents
    64, // MaxGeometryVaryingComponents
    128, // MaxTessControlInputComponents
    128, // MaxTessControlOutputComponents
    16, // MaxTessControlTextureImageUnits
    1024, // MaxTessControlUniformComponents
    4096, // MaxTessControlTotalOutputComponents
    128, // MaxTessEvaluationInputComponents
    128, // MaxTessEvaluationOutputComponents
    16, // MaxTessEvaluationTextureImageUnits
    1024, // MaxTessEvaluationUniformComponents
    120, // MaxTessPatchComponents
    32, // MaxPatchVertices
    64, // MaxTessGenLevel
    16, // MaxViewports
    0, // MaxVertexAtomicCounters
    0, // MaxTessControlAtomicCounters
    0, // MaxTessEvaluationAtomicCounters
    0, // MaxGeometryAtomicCounters
    8, // MaxFragmentAtomicCounters
    8, // MaxCombinedAtomicCounters
    1, // MaxAtomicCounterBindings
    0, // MaxVertexAtomicCounterBuffers
    0, // MaxTessControlAtomicCounterBuffers
    0, // MaxTessEvaluationAtomicCounterBuffers
    0, // MaxGeometryAtomicCounterBuffers
    1, // MaxFragmentAtomicCounterBuffers
    1, // MaxCombinedAtomicCounterBuffers
    16384, // MaxAtomicCounterBufferSize
    4, // MaxTransformFeedbackBuffers
    64, // MaxTransformFeedbackInterleavedComponents
    8, // MaxCullDistances
    8, // MaxCombinedClipAndCullDistances
    4, // MaxSamples
    // limits
    {
        1, // nonInductiveForLoops
        1, // whileLoops
        1, // doWhileLoops
        1, // generalUniformIndexing
        1, // generalAttributeMatrixVectorIndexing
        1, // generalVaryingIndexing
        1, // generalSamplerIndexing
        1, // generalVariableIndexing
        1, // generalConstantMatrixVectorIndexing
    }
};

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

    if (!glslShader.parse(&default_resources, 330, true, EShMsgDefault)) {

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

#else // Do not use glslang

bool sigma::validate_shader(const graphics::shader_data& shader, std::vector<boost::filesystem::path> source_filenames)
{
    return true;
}

#endif // USE_GLSLANG

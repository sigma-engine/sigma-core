#ifndef SIGMA_ENGINE_MATERIAL_HPP
#define SIGMA_ENGINE_MATERIAL_HPP

#include <sigma/opengl/shader_technique.hpp>

namespace sigma {
namespace opengl {
    class material : public shader_technique {
    public:
        static constexpr const char* WORLD_POSITION_OUTPUT_NAME = "position_output";
        static constexpr const char* DIFFUSE_COLOR_OUTPUT_NAME = "diffuse_output";
        static constexpr const char* NORMAL_OUTPUT_NAME = "normal_output";
        static constexpr const char* TEXTURE_COORDINATE_OUTPUT_NAME = "texture_coordinate_output";

        static constexpr const GLuint WORLD_POSITION_OUTPUT_LOCATION = 0;
        static constexpr const GLuint DIFFUSE_COLOR_OUTPUT_LOCATION = 1;
        static constexpr const GLuint NORMAL_OUTPUT_LOCATION = 2;
        static constexpr const GLuint TEXTURE_COORDINATE_OUTPUT_LOCATION = 3;

        material() = default;

        material(material&&) = default;

        material& operator=(material&&) = default;

        void link();

    private:
        material(const material&) = delete;
        material& operator=(const material&) = delete;
    };
}
}

#endif // SIGMA_ENGINE_MATERIAL_HPP

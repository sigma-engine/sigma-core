#ifndef SIGMA_FIVE_CONVERT_ASSIMP_CONVERTER_HPP
#define SIGMA_FIVE_CONVERT_ASSIMP_CONVERTER_HPP

#include <sigma/graphics/static_mesh.hpp>

#include <boost/filesystem/path.hpp>

#include <set>

namespace Assimp {
class Importer;
}

namespace sigma {
namespace convert {
    class assimp_converter {
    public:
        assimp_converter(boost::filesystem::path source_file);

        virtual ~assimp_converter();

        virtual const std::set<std::string>& static_mesh_names() const;

        virtual void convert_static_mesh(std::string name,
            graphics::static_mesh& mesh) const;

    private:
        std::unique_ptr<Assimp::Importer> importer_;
        std::set<std::string> static_mesh_names_;
    };
}
}

#endif // SIGMA_FIVE_CONVERT_ASSIMP_CONVERTER_HPP

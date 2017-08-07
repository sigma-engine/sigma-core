#include <sigma/graphics/static_mesh.hpp>
#include <sigma/util/filesystem.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <json/json.h>

#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/program_options.hpp>

#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

glm::vec3 convert_color(aiColor3D c)
{
    return glm::vec3(c.r, c.g, c.b);
}

glm::vec3 convert_3d(aiVector3D v)
{
    return { v.x, v.y, v.z };
}

// glm::quat convert_3d(aiQuaternion q)
// {
//     return glm::quat(q.w, q.x, q.y, q.z);
// }

glm::vec2 convert_2d(aiVector3D v)
{
    return glm::vec2(v.x, v.y);
}

std::string get_name(const aiMaterial* mat, const boost::filesystem::path& source_package_directory, const Json::Value& materials)
{
    aiString matName;
    mat->Get(AI_MATKEY_NAME, matName);
    std::string name = matName.C_Str();

    if (materials.isMember(name))
        name = materials[name].asString();

    if (name == "DefaultMaterial")
        return "default";

    if (boost::starts_with(name, "//"))
        return (source_package_directory / name.substr(2)).string();
    if (name[0] == '/')
        return name.substr(1);
    return (source_package_directory / name).string();
}

std::string get_name(const aiMesh* mesh)
{
    std::string name = mesh->mName.C_Str();
    if (name == "")
        return "unnamed";
    return name;
}

void convert_mesh(const aiScene* scene, const boost::filesystem::path& package_path, const Json::Value& materials, sigma::graphics::static_mesh& dest_mesh, const aiMesh* src_mesh)
{
    std::vector<sigma::graphics::static_mesh::vertex> submesh_vertices(src_mesh->mNumVertices);
    for (unsigned int j = 0; j < src_mesh->mNumVertices; ++j) {
        auto pos = src_mesh->mVertices[j];
        submesh_vertices[j].position = convert_3d(pos);

        if (src_mesh->HasNormals()) {
            submesh_vertices[j].normal = convert_3d(src_mesh->mNormals[j]);
        }

        if (src_mesh->HasTangentsAndBitangents()) {
            submesh_vertices[j].tangent = convert_3d(src_mesh->mTangents[j]);
            submesh_vertices[j].bitangent = convert_3d(src_mesh->mBitangents[j]);
        }

        if (src_mesh->HasTextureCoords(0)) {
            submesh_vertices[j].texcoord = convert_2d(src_mesh->mTextureCoords[0][j]);
        }
    }

    std::vector<sigma::graphics::static_mesh::triangle> submesh_triangles(src_mesh->mNumFaces);
    for (unsigned int j = 0; j < src_mesh->mNumFaces; ++j) {
        aiFace f = src_mesh->mFaces[j];
        for (unsigned int k = 0; k < 3; ++k)
            submesh_triangles[j][k] = f.mIndices[k] + static_cast<unsigned int>(dest_mesh.vertices.size());
    }

    std::string material_name = get_name(scene->mMaterials[src_mesh->mMaterialIndex], package_path.parent_path().parent_path(), materials);

    // TODO warn if material slot has been used.
    dest_mesh.materials[boost::filesystem::path{ "material" } / material_name] = std::make_pair(dest_mesh.triangles.size(), submesh_triangles.size());

    dest_mesh.vertices.reserve(dest_mesh.vertices.size() + submesh_vertices.size());
    dest_mesh.vertices.insert(dest_mesh.vertices.end(), submesh_vertices.begin(), submesh_vertices.end());

    dest_mesh.triangles.reserve(dest_mesh.triangles.size() + submesh_triangles.size());
    dest_mesh.triangles.insert(dest_mesh.triangles.end(), submesh_triangles.begin(), submesh_triangles.end());
}

int main(int argc, char const* argv[])
{
    boost::program_options::options_description global{ "options" };
    boost::program_options::positional_options_description positional;

    // clang-format off
    global.add_options()
    ("source-directory,H", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level package directory.")
    ("build-directory,B", boost::program_options::value<std::string>()->default_value(boost::filesystem::current_path().string()), "The top level project build directory.")
    ("rid,r", boost::program_options::value<std::string>(), "Override the auto generated resource identifier.")
    ("dependency,M", "List dependencies of the static mesh conversion.")
    ("source-file,c", boost::program_options::value<std::string>()->required(), "The static mesh file to convert.");
    // clang-format on

    positional.add("source-file", -1);

    try {
        boost::program_options::variables_map vm;
        boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(global).positional(positional).run(), vm);
        boost::program_options::notify(vm);

        auto source_file = boost::filesystem::canonical(vm["source-file"].as<std::string>());
        auto material_file = source_file.parent_path() / "materials";
        auto source_directory = boost::filesystem::canonical(vm["source-directory"].as<std::string>());
        auto build_directory = boost::filesystem::canonical(vm["build-directory"].as<std::string>());

        boost::filesystem::path package_path;
        if (vm.count("rid"))
            package_path = vm["rid"].as<std::string>();
        else
            package_path = sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");

        auto rid = "static_mesh" / package_path;
        auto output_file = build_directory / "data" / rid;

        if (vm.count("dependency")) {
            boost::filesystem::path dependency_path = output_file;
            dependency_path.replace_extension(source_file.extension().string() + ".dependency");
            std::ofstream dep{ dependency_path.string() };

            std::regex re{ "[^a-zA-Z0-9]" };
            dep << "set(" << std::regex_replace(rid.string(), re, "_") << "_DEPENDS\n";
            dep << ")\n";
            return 0;
        }

        Json::Value materials(Json::objectValue);
        if (boost::filesystem::exists(material_file)) {
            std::ifstream file(material_file.string());
            file >> materials;
        }

        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(source_file.c_str(),
            aiProcess_CalcTangentSpace
                | aiProcess_JoinIdenticalVertices
                | aiProcess_Triangulate
                // | aiProcess_LimitBoneWeights
                | aiProcess_ValidateDataStructure
                | aiProcess_ImproveCacheLocality
                // | aiProcess_RemoveRedundantMaterials
                | aiProcess_SortByPType
                | aiProcess_FindDegenerates
                | aiProcess_FindInvalidData
                // | aiProcess_GenUVCoords
                // | aiProcess_FindInstances
                | aiProcess_FlipUVs
                | aiProcess_CalcTangentSpace
                // | aiProcess_MakeLeftHanded
                | aiProcess_RemoveComponent
                // | aiProcess_GenNormals
                // | aiProcess_GenSmoothNormals
                | aiProcess_SplitLargeMeshes
                | aiProcess_PreTransformVertices
                // | aiProcess_FixInfacingNormals
                // | aiProcess_TransformUVCoords
                // | aiProcess_ConvertToLeftHanded
                | aiProcess_OptimizeMeshes
            // | aiProcess_OptimizeGraph
            // | aiProcess_FlipWindingOrder
            // | aiProcess_SplitByBoneCount
            // | aiProcess_Debone
        );

        if (scene == nullptr)
            throw std::runtime_error(importer.GetErrorString());

        sigma::graphics::static_mesh dest_mesh;
        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            if (!boost::ends_with(get_name(scene->mMeshes[i]), "_high"))
                convert_mesh(scene, package_path, materials, dest_mesh, scene->mMeshes[i]);
        }
        dest_mesh.vertices.shrink_to_fit();
        dest_mesh.triangles.shrink_to_fit();

        auto output_directory = output_file.parent_path();
        if (!boost::filesystem::exists(output_directory))
            boost::filesystem::create_directories(output_directory);

        std::ofstream stream(output_file.string(), std::ios::binary | std::ios::out);
        boost::archive::binary_oarchive oa(stream);
        oa << dest_mesh;
    } catch (boost::program_options::error& e) {
        std::cerr << "mdlcc\nerror: " << e.what() << '\n';
        std::cerr << global << '\n';
        return -1;
    } catch (const std::exception& e) {
        std::cerr << "mdlcc\nerror: " << e.what() << '\n';
        return -1;
    }
    return 0;
}

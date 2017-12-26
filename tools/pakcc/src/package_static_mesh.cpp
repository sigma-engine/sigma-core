#include <package_static_mesh.hpp>

#include <sigma/util/filesystem.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <set>
#include <string>

glm::vec3 convert_color(aiColor3D c)
{
    return glm::vec3(c.r, c.g, c.b);
}

glm::vec3 convert_3d(aiVector3D v)
{
    return { v.x, v.y, v.z };
}

glm::vec2 convert_2d(aiVector3D v)
{
    return glm::vec2(v.x, v.y);
}

std::string get_name(const aiMaterial* mat, const boost::filesystem::path& package_directory)
{
    aiString matName;
    mat->Get(AI_MATKEY_NAME, matName);
    std::string name = matName.C_Str();

    if (name == "DefaultMaterial")
        return "default";

    if (boost::starts_with(name, "//"))
        return (package_directory / name.substr(2)).string();
    if (name[0] == '/')
        return name.substr(1);
    return (package_directory / name).string();
}

std::string get_name(const aiMesh* mesh)
{
    std::string name = mesh->mName.C_Str();
    if (name == "")
        return "unnamed";
    return name;
}

void package_static_mesh(
    const sigma::resource::database<sigma::graphics::material>& material_database,
    sigma::resource::database<sigma::graphics::static_mesh>& static_mesh_database,
    const boost::filesystem::path& source_directory,
    const boost::filesystem::path& source_file)
{
    // TODO FEATURE add settings?
    auto package_path = sigma::filesystem::make_relative(source_directory, source_file).replace_extension("");
    auto package_directory = package_path.parent_path();
    auto rid = "static_mesh" / package_path;

    if (static_mesh_database.contains({ rid })) {
        auto h = static_mesh_database.handle_for({ rid });

        auto source_file_time = boost::filesystem::last_write_time(source_file);
        auto resource_time = static_mesh_database.last_modification_time(h);
        if (source_file_time <= resource_time)
            return;
    }

    std::cout << "packaging: " << rid << "\n";

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
        if (boost::ends_with(get_name(scene->mMeshes[i]), "_high"))
            continue;

        auto src_mesh = scene->mMeshes[i];
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

        std::string material_name = get_name(scene->mMaterials[src_mesh->mMaterialIndex], package_directory);

        // TODO warn if material slot has been used.
        // TODO error if missing material.
        dest_mesh.materials.push_back(material_database.handle_for({ boost::filesystem::path{ "material" } / material_name }));
        dest_mesh.material_slots.push_back(std::make_pair(dest_mesh.triangles.size(), submesh_triangles.size()));

        dest_mesh.vertices.reserve(dest_mesh.vertices.size() + submesh_vertices.size());
        dest_mesh.vertices.insert(dest_mesh.vertices.end(), submesh_vertices.begin(), submesh_vertices.end());

        dest_mesh.triangles.reserve(dest_mesh.triangles.size() + submesh_triangles.size());
        dest_mesh.triangles.insert(dest_mesh.triangles.end(), submesh_triangles.begin(), submesh_triangles.end());
    }
    dest_mesh.vertices.shrink_to_fit();
    dest_mesh.triangles.shrink_to_fit();

    static_mesh_database.insert({ rid }, dest_mesh);
}

void package_static_meshes(
    const sigma::resource::database<sigma::graphics::material>& material_database,
    sigma::resource::database<sigma::graphics::static_mesh>& static_mesh_database,
    const boost::filesystem::path& source_directory)
{
    static const std::set<std::string> supported_ext{
        ".3ds", ".dae", ".fbx", ".ifc-step", ".ase", ".dxf", ".hmp", ".md2",
        ".md3", ".md5", ".mdc", ".mdl", ".nff", ".ply", ".stl", ".x", ".obj",
        ".opengex", ".smd", ".lwo", ".lxo", ".lws", ".ter", ".ac3d", ".ms3d",
        ".cob", ".q3bsp", ".xgl", ".csm", ".bvh", ".b3d", ".ndo", ".q3d",
        ".gltf", ".3mf"
    };

    boost::filesystem::recursive_directory_iterator it{ source_directory };
    boost::filesystem::recursive_directory_iterator end;
    for (; it != end; ++it) {
        auto path = it->path();
        if (sigma::filesystem::is_hidden(path)) {
            if (boost::filesystem::is_directory(path))
                it.no_push();
            continue;
        }

        auto ext = boost::algorithm::to_lower_copy(path.extension().string());
        auto ext_it = supported_ext.find(ext);
        if (boost::filesystem::is_regular_file(path) && ext_it != supported_ext.cend()) {
            try {
                package_static_mesh(material_database, static_mesh_database, source_directory, path);
            } catch (const std::runtime_error& e) {
                std::cerr << "error: " << e.what() << '\n';
            }
        }
    }
}

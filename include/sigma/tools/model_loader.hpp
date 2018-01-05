#ifndef SIGMA_TOOLS_MODEL_LOADER_HPP
#define SIGMA_TOOLS_MODEL_LOADER_HPP

#include <sigma/blueprint.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/graphics/technique.hpp>
#include <sigma/tools/json_conversion.hpp>
#include <sigma/tools/packager.hpp>
#include <sigma/transform.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/type_sequence.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <boost/algorithm/string.hpp>

#include <iostream>
#include <set>
#include <string>

namespace sigma {
namespace tools {
    glm::vec3 convert_color(aiColor3D c)
    {
        return glm::vec3(c.r, c.g, c.b);
    }

    glm::vec3 convert_3d(aiVector3D v)
    {
        return { v.x, v.y, v.z };
    }

    glm::quat convert(aiQuaternion q)
    {
        return { q.w, q.x, q.y, q.z };
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

    std::string get_name(const aiNode* node)
    {
        std::string name = node->mName.C_Str();
        if (name == "")
            return "unnamed";
        return name;
    }

    void convert_static_mesh(const boost::filesystem::path& package_directory, const resource::cache<graphics::material>& material_cache, const aiScene* scene, const aiMesh* src_mesh, graphics::static_mesh& dest_mesh)
    {
        std::vector<graphics::static_mesh::vertex> submesh_vertices(src_mesh->mNumVertices);
        std::vector<graphics::static_mesh::triangle> submesh_triangles(src_mesh->mNumFaces);

        for (unsigned int j = 0; j < src_mesh->mNumVertices; ++j) {
            auto pos = src_mesh->mVertices[j];
            submesh_vertices[j].position = convert_3d(pos);
            dest_mesh.radius = std::max(dest_mesh.radius, glm::length(submesh_vertices[j].position));

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

        for (unsigned int j = 0; j < src_mesh->mNumFaces; ++j) {
            aiFace f = src_mesh->mFaces[j];
            for (unsigned int k = 0; k < 3; ++k)
                submesh_triangles[j][k] = f.mIndices[k] + static_cast<unsigned int>(dest_mesh.vertices.size());
        }

        std::string material_name = get_name(scene->mMaterials[src_mesh->mMaterialIndex], package_directory);

        // TODO warn if material slot has been used.
        boost::filesystem::path material_prefix{ resource_shortname(graphics::material) };
        dest_mesh.materials.push_back(material_cache.handle_for({ material_prefix / material_name }));
        dest_mesh.material_slots.push_back(std::make_pair(dest_mesh.triangles.size(), submesh_triangles.size()));

        dest_mesh.vertices.reserve(dest_mesh.vertices.size() + submesh_vertices.size());
        dest_mesh.vertices.insert(dest_mesh.vertices.end(), submesh_vertices.begin(), submesh_vertices.end());

        dest_mesh.triangles.reserve(dest_mesh.triangles.size() + submesh_triangles.size());
        dest_mesh.triangles.insert(dest_mesh.triangles.end(), submesh_triangles.begin(), submesh_triangles.end());
    }

    void convert_static_mesh(
        const boost::filesystem::path& package_directory,
        const resource::cache<graphics::material>& material_cache,
        const aiScene* scene,
        const aiMesh* src_mesh,
        graphics::static_mesh& dest_mesh);

    template <class PackageSettings, class ContextType, class ComponentSet>
    class model_loader : public resource_loader<PackageSettings, ContextType> {
    public:
        using component_set_type = ComponentSet;
        using blueprint_type = blueprint<component_set_type>;

        model_loader(ContextType& ctx)
            : context_{ ctx }
        {
        }

        virtual ~model_loader() = default;

        bool supports_filetype(const std::string& ext) const
        {
            static const std::set<std::string> supported_extensions{
                ".3ds", ".dae", ".fbx", ".ifc-step", ".ase", ".dxf", ".hmp", ".md2",
                ".md3", ".md5", ".mdc", ".mdl", ".nff", ".ply", ".stl", ".x", ".obj",
                ".opengex", ".smd", ".lwo", ".lxo", ".lws", ".ter", ".ac3d", ".ms3d",
                ".cob", ".q3bsp", ".xgl", ".csm", ".bvh", ".b3d", ".ndo", ".q3d",
                ".gltf", ".3mf"
            };
            return supported_extensions.count(ext) > 0;
        }

        void load(const PackageSettings& package_settings, const boost::filesystem::path& source_directory, const std::string& ext, const boost::filesystem::path& source_file)
        {
            auto package_path = filesystem::make_relative(source_directory, source_file).replace_extension("");
            auto model_settings_path = source_file.parent_path() / (source_file.stem().string() + ".mdl");
            auto scene_settings_path = source_file.parent_path() / (source_file.stem().string() + ".bpt");

            if (boost::filesystem::exists(scene_settings_path)) {
                load_as_blueprint(
                    package_path,
                    source_file,
                    scene_settings_path);
            } else {
                load_as_static_mesh(
                    package_path,
                    source_file,
                    model_settings_path);
            }
        }

    private:
        void convert_entity(const Json::Value& json_entity, typename blueprint_type::entity_type& blueprint_entity)
        {
            component_set_type::for_each([&](auto component_tag) {
                using component_type = typename decltype(component_tag)::type;
                if (json_entity.isMember(component_name(component_type))) {
                    component_type cmp;
                    json::from_json(context_, json_entity[component_name(component_type)], cmp);
                    blueprint_entity.push_back(cmp);
                }
            });
        }

        void convert_node(const resource::cache<graphics::static_mesh>& static_mesh_cache,
            const boost::filesystem::path& package_path,
            const aiScene* scene,
            std::string parent_name,
            const aiMatrix4x4 parent_matrix,
            const aiNode* root,
            Json::Value& blueprint_settings,
            blueprint_type& blueprint)
        {
            auto node_name = parent_name + (parent_name.size() > 0 ? "/" : "") + get_name(root);
            auto matrix = parent_matrix * root->mTransformation;

            aiVector3D scale;
            aiQuaternion rotation;
            aiVector3D position;
            matrix.Decompose(scale, rotation, position);

            for (unsigned int j = 0; j < root->mNumMeshes; ++j) {
                std::string entity_name = node_name;
                if (j > 0)
                    entity_name += std::to_string(j);

                auto mesh = scene->mMeshes[root->mMeshes[j]];
                auto rid = resource_shortname(graphics::static_mesh) / package_path / (get_name(mesh) + std::to_string(root->mMeshes[j]));

                graphics::static_mesh_instance mshinst{
                    static_mesh_cache.handle_for({ rid })
                };

                transform txfrom{
                    convert_3d(position),
                    convert(rotation),
                    convert_3d(scale)
                };

                typename blueprint_type::entity_type blueprint_entity{ txfrom, mshinst };
                if (blueprint_settings.isMember(entity_name)) {
                    Json::Value json_entity;
                    blueprint_settings.removeMember(entity_name, &json_entity);
                    convert_entity(json_entity, blueprint_entity);
                }
                blueprint.entities.push_back(blueprint_entity);
            }

            for (unsigned int i = 0; i < root->mNumChildren; ++i) {
                auto node = root->mChildren[i];
                convert_node(static_mesh_cache, package_path, scene, node_name, matrix, node, blueprint_settings, blueprint);
            }
        }

        void load_as_blueprint(
            const boost::filesystem::path& package_path,
            const boost::filesystem::path& source_file,
            const boost::filesystem::path& settings_path)
        {
            auto& material_cache = context_.template get_cache<graphics::material>();
            auto& static_mesh_cache = context_.template get_cache<graphics::static_mesh>();
            auto& blueprint_cache = context_.template get_cache<blueprint_type>();

            auto package_directory = package_path.parent_path();
            auto blueprint_rid = resource_shortname(blueprint_type) / package_path;

            if (blueprint_cache.contains({ blueprint_rid })) {
                auto h = blueprint_cache.handle_for({ blueprint_rid });

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto settings_time = source_file_time;
                if (boost::filesystem::exists(settings_path))
                    settings_time = boost::filesystem::last_write_time(settings_path);

                auto resource_time = blueprint_cache.last_modification_time(h);
                if (source_file_time <= resource_time && settings_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << blueprint_rid << "\n";

            Json::Value settings(Json::objectValue);
            if (boost::filesystem::exists(settings_path)) {
                std::ifstream file(settings_path.string());
                file >> settings;
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
                    // | aiProcess_SplitLargeMeshes
                    // | aiProcess_PreTransformVertices
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

            for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
                if (boost::ends_with(get_name(scene->mMeshes[i]), "_high"))
                    continue;

                auto rid = resource_shortname(graphics::static_mesh) / package_path / (get_name(scene->mMeshes[i]) + std::to_string(i));
                std::cout << "packaging: " << rid << "\n";

                graphics::static_mesh dest_mesh;
                convert_static_mesh(package_directory, material_cache, scene, scene->mMeshes[i], dest_mesh);
                dest_mesh.vertices.shrink_to_fit();
                dest_mesh.triangles.shrink_to_fit();

                static_mesh_cache.insert({ rid }, dest_mesh, true);
            }

            blueprint_type blueprint;
            convert_node(static_mesh_cache, package_path, scene, "", aiMatrix4x4{}, scene->mRootNode, settings, blueprint);

            for (const auto& json_enttiy : settings) {
                typename blueprint_type::entity_type blueprint_entity;
                convert_entity(json_enttiy, blueprint_entity);
                blueprint.entities.push_back(blueprint_entity);
            }

            blueprint_cache.insert({ blueprint_rid }, blueprint, true);
        }

        void load_as_static_mesh(
            const boost::filesystem::path& package_path,
            const boost::filesystem::path& source_file,
            const boost::filesystem::path& settings_path)
        {
            auto& material_cache = context_.template get_cache<graphics::material>();
            auto& static_mesh_cache = context_.template get_cache<graphics::static_mesh>();

            auto package_directory = package_path.parent_path();
            auto rid = resource_shortname(graphics::static_mesh) / package_path;

            if (static_mesh_cache.contains({ rid })) {
                auto h = static_mesh_cache.handle_for({ rid });

                auto source_file_time = boost::filesystem::last_write_time(source_file);
                auto settings_time = source_file_time;
                if (boost::filesystem::exists(settings_path))
                    settings_time = boost::filesystem::last_write_time(settings_path);

                auto resource_time = static_mesh_cache.last_modification_time(h);
                if (source_file_time <= resource_time && settings_time <= resource_time)
                    return;
            }

            std::cout << "packaging: " << rid << "\n";

            Json::Value settings(Json::objectValue);
            if (boost::filesystem::exists(settings_path)) {
                std::ifstream file(settings_path.string());
                file >> settings;
            }

            // TODO FEATURE add settings.

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
                    // | aiProcess_SplitLargeMeshes
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

            graphics::static_mesh dest_mesh;
            for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
                if (boost::ends_with(get_name(scene->mMeshes[i]), "_high"))
                    continue;
                convert_static_mesh(package_directory, material_cache, scene, scene->mMeshes[i], dest_mesh);
            }
            dest_mesh.vertices.shrink_to_fit();
            dest_mesh.triangles.shrink_to_fit();

            static_mesh_cache.insert({ rid }, dest_mesh, true);
        }

    private:
        ContextType& context_;
    };
}
}

#endif // SIGMA_TOOLS_MODEL_MODEL_HPP

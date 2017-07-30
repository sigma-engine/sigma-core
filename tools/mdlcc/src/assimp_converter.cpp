#include <assimp_converter.hpp>

#include <sigma/graphics/directional_light.hpp>
#include <sigma/graphics/point_light.hpp>
#include <sigma/graphics/spot_light.hpp>
#include <sigma/graphics/static_mesh.hpp>
#include <sigma/graphics/static_mesh_instance.hpp>
#include <sigma/transform.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/json_conversion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <json/json.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/operations.hpp>

#include <unordered_map>

namespace sigma {

struct converter {
    virtual glm::vec3 convert_color(aiColor3D c)
    {
        return glm::vec3(c.r, c.g, c.b);
    }

    virtual glm::vec3 convert_3d(aiVector3D v)
    {
        return { v.x, v.y, v.z };
    }

    virtual glm::quat convert_3d(aiQuaternion q)
    {
        return glm::quat(q.w, q.x, q.y, q.z);
    }

    virtual glm::vec2 convert_2d(aiVector3D v)
    {
        return glm::vec2(v.x, v.y);
    }
};

struct blender_converter : public converter {
    virtual glm::vec3 convert_color(aiColor3D c) override
    {
        return glm::vec3(c.r, c.g, c.b);
    }

    virtual glm::vec3 convert_3d(aiVector3D v) override
    {
        return { v.x, v.z, -v.y };
    }

    virtual glm::quat convert_3d(aiQuaternion q) override
    {
        return glm::quat(q.w, q.x, q.z, -q.y);
    }

    virtual glm::vec2 convert_2d(aiVector3D v) override
    {
        return glm::vec2(v.x, v.y);
    }
};

assimp_converter::assimp_converter(boost::filesystem::path package_root, boost::filesystem::path source_file)
    : root_directroy_(filesystem::make_relative(package_root, source_file).parent_path())
    , source_file_(source_file)
    , importer_(std::make_unique<Assimp::Importer>())
{
    resource_id_ = sigma::filesystem::make_relative(package_root, source_file_).replace_extension("");
    auto filename = source_file_.string();

    if (boost::algorithm::ends_with(filename, ".blend"))
        converter_ = std::make_unique<blender_converter>();
    else
        converter_ = std::make_unique<converter>();

    const aiScene* scene = importer_->ReadFile(filename.c_str(),
        aiProcess_CalcTangentSpace
            | aiProcess_JoinIdenticalVertices
            | aiProcess_Triangulate
            | aiProcess_LimitBoneWeights
            | aiProcess_ValidateDataStructure
            | aiProcess_ImproveCacheLocality
            //| aiProcess_RemoveRedundantMaterials
            | aiProcess_SortByPType
            | aiProcess_FindDegenerates
            | aiProcess_FindInvalidData
            // | aiProcess_GenUVCoords
            | aiProcess_FindInstances
            | aiProcess_FlipUVs
            | aiProcess_CalcTangentSpace
        // | aiProcess_MakeLeftHanded
        // | aiProcess_RemoveComponent
        // | aiProcess_GenNormals
        // | aiProcess_GenSmoothNormals
        // | aiProcess_SplitLargeMeshes
        // | aiProcess_PreTransformVertices
        // | aiProcess_FixInfacingNormals
        // | aiProcess_TransformUVCoords
        // | aiProcess_ConvertToLeftHanded
        // | aiProcess_OptimizeMeshes
        // | aiProcess_OptimizeGraph
        // | aiProcess_FlipWindingOrder
        // | aiProcess_SplitByBoneCount
        // | aiProcess_Debone
        );
    if (scene == nullptr) {
        // TODO error handling
        std::cout << importer_->GetErrorString() << '\n';
        return;
    }

    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        if (is_source(scene->mMaterials[i])) {
            std::string name = get_name(scene->mMaterials[i]);
            material_names_.insert(name);
        }
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        std::string name = get_name(scene->mMeshes[i]);
        // if (!scene->mMeshes[i]->HasBones())
        static_mesh_names_.insert(name);
        // else
        //     static_mesh_names_.erase(name);
    }

    // TODO recursive
    for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; ++i) {
        object_names_.insert(get_name(scene->mRootNode->mChildren[i]));
    }

    // TODO skeletal mesehes.
    // TODO the whole scene.
    // TODO animations.
    // TODO textures?
}

assimp_converter::~assimp_converter() {}

const std::set<std::string>& assimp_converter::material_names() const
{
    return material_names_;
}

const std::set<std::string>& assimp_converter::static_mesh_names() const
{
    return static_mesh_names_;
}

const std::set<std::string>& assimp_converter::scene_object_names() const
{
    return object_names_;
}

void assimp_converter::convert_material(std::string name, Json::Value& material) const
{
    const aiScene* aiScene = importer_->GetScene();

    std::unordered_map<aiTextureType, std::string> texture_type_map = {
        { aiTextureType_DIFFUSE, "basecolor_map" },
        { aiTextureType_SPECULAR, "roughness_map" },
        { aiTextureType_AMBIENT, "metallic_map" },
        { aiTextureType_EMISSIVE, "emissive_map" },
        { aiTextureType_HEIGHT, "normal_map" },
        { aiTextureType_NORMALS, "normal_map" },
        { aiTextureType_SHININESS, "roughness_map" },
        { aiTextureType_OPACITY, "opacity_map" },
        { aiTextureType_DISPLACEMENT, "displacement_map" },
        { aiTextureType_LIGHTMAP, "lightmap_map" },
        { aiTextureType_REFLECTION, "reflection_map" }
    };

    material["vertex"] = "default";
    material["fragment"] = "default_basic";
    for (unsigned int i = 0; i < aiScene->mNumMaterials; ++i) {
        auto aimat = aiScene->mMaterials[i];
        if (get_name(aimat) != name)
            continue;

        aiColor3D color;
        if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_DIFFUSE, color))
            json::to_json(converter_->convert_color(color), material["basecolor"]);
        if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_SPECULAR, color))
            json::to_json(converter_->convert_color(color).x, material["roughness"]);
        if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_AMBIENT, color))
            json::to_json(converter_->convert_color(color).x, material["metalness"]);
        if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_EMISSIVE, color))
            json::to_json(converter_->convert_color(color), material["emissiveColor"]);
        if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_TRANSPARENT, color))
            json::to_json(converter_->convert_color(color), material["transparentColor"]);
        if (AI_SUCCESS == aimat->Get(AI_MATKEY_COLOR_REFLECTIVE, color))
            json::to_json(converter_->convert_color(color), material["reflectiveColor"]);

        for (auto texture_type : texture_type_map) {
            if (aimat->GetTextureCount(texture_type.first) > 0) {
                material["textures"][texture_type.second] = get_name(aimat, texture_type.first);
                material["fragment"] = "default";
            }
        }
        return;
    }
}

void assimp_converter::convert_static_mesh(std::string name, graphics::static_mesh& mesh) const
{
    const aiScene* aiScene = importer_->GetScene();

    for (unsigned int i = 0; i < aiScene->mNumMeshes; ++i) {
        const aiMesh* aimesh = aiScene->mMeshes[i];
        if (name != get_name(aimesh))
            continue;

        std::vector<sigma::graphics::static_mesh::vertex> submesh_vertices(aimesh->mNumVertices);
        for (unsigned int j = 0; j < aimesh->mNumVertices; ++j) {
            auto pos = aimesh->mVertices[j];
            submesh_vertices[j].position = converter_->convert_3d(pos);

            if (aimesh->HasNormals()) {
                auto nor = aimesh->mNormals[j];
                submesh_vertices[j].normal = converter_->convert_3d(nor);
            }

            if (aimesh->HasTangentsAndBitangents()) {
                auto tan = aimesh->mTangents[j];
                auto bitan = aimesh->mBitangents[j];
                submesh_vertices[j].tangent = converter_->convert_3d(tan);
                submesh_vertices[j].bitangent = converter_->convert_3d(bitan);
            }
            if (aimesh->HasTextureCoords(0)) {
                auto tex = converter_->convert_2d(aimesh->mTextureCoords[0][j]);
                submesh_vertices[j].texcoord = tex;
            }
        }

        std::vector<sigma::graphics::static_mesh::triangle> submesh_triangles(aimesh->mNumFaces);
        for (unsigned int j = 0; j < aimesh->mNumFaces; ++j) {
            aiFace f = aimesh->mFaces[j];
            for (unsigned int k = 0; k < 3; ++k)
                submesh_triangles[j][k] = f.mIndices[k] + static_cast<unsigned int>(mesh.vertices.size());
        }

        std::string material_name = get_name(aiScene->mMaterials[aimesh->mMaterialIndex]);

        // TODO warn if material slot has been used.
        mesh.materials[boost::filesystem::path{ "material" } / material_name] = std::make_pair(mesh.triangles.size(), aimesh->mNumFaces);

        mesh.vertices.reserve(mesh.vertices.size() + aimesh->mNumVertices);
        mesh.vertices.insert(mesh.vertices.end(), submesh_vertices.begin(), submesh_vertices.end());

        mesh.triangles.reserve(mesh.triangles.size() + submesh_triangles.size());
        mesh.triangles.insert(mesh.triangles.end(), submesh_triangles.begin(), submesh_triangles.end());
    }
    mesh.vertices.shrink_to_fit();
    mesh.triangles.shrink_to_fit();
}

void assimp_converter::convert_object(std::string name, Json::Value& entity) const
{
    const aiScene* aiScene = importer_->GetScene();
    const aiNode* ainode = aiScene->mRootNode->FindNode(name.c_str());
    aiVector3D aiposition;
    aiQuaternion airotation;
    aiVector3D aiscale;
    ainode->mTransformation.Decompose(aiscale, airotation, aiposition);
    aiscale.y *= -1;

    sigma::transform txform;
    txform.position = converter_->convert_3d(aiposition);
    txform.rotation = converter_->convert_3d(airotation);
    txform.scale = converter_->convert_3d(aiscale);
    json::to_json(txform, entity["sigma::transform"]);

    if (ainode->mNumMeshes > 0) {
        // TODO warn if more than one mesh per object
        const aiMesh* aimesh = aiScene->mMeshes[ainode->mMeshes[0]];

        sigma::graphics::static_mesh_instance inst;
        inst.mesh_id = boost::filesystem::path{ "static_mesh" } / resource_id_ / get_name(aimesh);
        json::to_json(inst, entity["sigma::graphics::static_mesh_instance"]);
    }

    for (unsigned int i = 0; i < aiScene->mNumLights; ++i) {
        const aiLight* ailight = aiScene->mLights[i];
        if (name == ailight->mName.C_Str()) {
            auto color = converter_->convert_color(ailight->mColorDiffuse);
            float intensity = color[0]; // TOOD check intensity and color
            color /= intensity;

            switch (ailight->mType) {
            case aiLightSource_POINT: {
                sigma::graphics::point_light light;
                light.color = color;
                light.intensity = intensity;
                json::to_json(light, entity["sigma::graphics::point_light"]);
                break;
            }
            case aiLightSource_DIRECTIONAL: {
                sigma::graphics::directional_light light;
                light.color = color;
                light.intensity = intensity;
                json::to_json(light, entity["sigma::graphics::directional_light"]);
                break;
            }
            case aiLightSource_SPOT: {
                sigma::graphics::spot_light light;
                light.color = color;
                light.intensity = intensity;
                light.cutoff = ailight->mAngleOuterCone;
                json::to_json(light, entity["sigma::graphics::spot_light"]);
                break;
            }
            default: // TODO more lights
                break;
            }
            break;
        }
    }
}

std::string assimp_converter::get_name(const aiMaterial* mat, int texture_type) const
{
    aiString texture;
    mat->GetTexture((aiTextureType)texture_type, 0, &texture);
    std::string name = texture.C_Str();
    boost::algorithm::replace_all(name, "\\", "/");

    boost::filesystem::path texture_path = root_directroy_ / name;
    if (boost::starts_with(name, "//"))
        texture_path = root_directroy_ / name.substr(2);
    else if (name[0] == '/')
        texture_path = name.substr(1);
    else if (!boost::filesystem::exists(texture_path))
        texture_path = name;

    return texture_path.replace_extension("").string();
}

bool assimp_converter::is_source(const aiMaterial* mat) const
{
    aiString matName;
    mat->Get(AI_MATKEY_NAME, matName);
    std::string name = matName.C_Str();
    return name[0] != '/' && name != "DefaultMaterial";
}

std::string assimp_converter::get_name(const aiMaterial* mat) const
{
    aiString matName;
    mat->Get(AI_MATKEY_NAME, matName);
    std::string name = matName.C_Str();
    if (name == "DefaultMaterial")
        return "default";

    if (boost::starts_with(name, "//"))
        return (root_directroy_ / name.substr(2)).string();
    if (name[0] == '/')
        return name.substr(1);
    return (root_directroy_ / name).string();
}

std::string assimp_converter::get_name(const aiMesh* mesh) const
{
    std::string name = mesh->mName.C_Str();
    if (name == "")
        return "unnamed";
    return name;
}

std::string assimp_converter::get_name(const aiNode* node) const
{
    std::string name = node->mName.C_Str();
    if (name == "")
        return "unnamed";
    return name;
}
}

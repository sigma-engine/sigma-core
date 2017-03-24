#include <assimp_converter.hpp>

#include <sigma/graphics/static_mesh.hpp>
#include <sigma/util/filesystem.hpp>
#include <sigma/util/json_conversion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <json/json.h>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <boost/algorithm/string/predicate.hpp>

#include <iostream>
#include <unordered_map>

namespace sigma {

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

std::string get_name(const aiMaterial* mat)
{
    aiString matName;
    mat->Get(AI_MATKEY_NAME, matName);
    return matName.C_Str();
}

glm::vec3 convert_color(aiColor3D c)
{
    return glm::vec3(c.r, c.g, c.b);
}

glm::vec3 convert_3d(aiVector3D v)
{
    return { v.x, v.z, -v.y };
}

glm::quat convert_3d(aiQuaternion q)
{
    return glm::quat(q.w, q.x, q.z, -q.y);
}

glm::vec2 convert_2d(aiVector3D v)
{
    return glm::vec2(v.x, v.y);
}

assimp_converter::assimp_converter(boost::filesystem::path source_file)
    : source_file_(source_file)
    , importer_(std::make_unique<Assimp::Importer>())
{
    auto filename = source_file_.string();
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
            | aiProcess_GenUVCoords
            | aiProcess_FindInstances
            | aiProcess_FlipUVs
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

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        std::string name = get_name(scene->mMeshes[i]);
        if (!scene->mMeshes[i]->HasBones())
            static_mesh_names_.insert(name);
        else
            static_mesh_names_.erase(name);
    }

    // TODO recursive
    for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; ++i) {
        object_names_.insert(get_name(scene->mRootNode->mChildren[i]));
    }

    // TODO skeletal mesehes.
    // TODO matericonvertals
    // TODO the whole scene.
    // TODO animations.
    // TODO textures?
}

assimp_converter::~assimp_converter() {}

const std::set<std::string>& assimp_converter::static_mesh_names() const
{
    return static_mesh_names_;
}

const std::set<std::string>& assimp_converter::scene_object_names() const
{
    return object_names_;
}

void assimp_converter::convert_static_mesh(std::string name, graphics::static_mesh_data& mesh) const
{
    const aiScene* aiScene = importer_->GetScene();

    for (unsigned int i = 0; i < aiScene->mNumMeshes; ++i) {
        const aiMesh* aimesh = aiScene->mMeshes[i];
        if (name != get_name(aimesh))
            continue;

        std::vector<sigma::graphics::static_mesh_data::vertex> submesh_vertices(aimesh->mNumVertices);
        for (unsigned int j = 0; j < aimesh->mNumVertices; ++j) {
            auto pos = aimesh->mVertices[j];
            submesh_vertices[j].position = convert_3d(pos);

            if (aimesh->HasNormals()) {
                auto nor = aimesh->mNormals[j];
                submesh_vertices[j].normal = convert_3d(nor);
            }

            if (aimesh->HasTangentsAndBitangents()) {
                auto tan = aimesh->mTangents[j];
                submesh_vertices[j].tangent = convert_3d(tan);
            }
            if (aimesh->HasTextureCoords(0)) {
                auto tex = convert_2d(aimesh->mTextureCoords[0][j]);
                submesh_vertices[j].texcoord = tex;
            }
        }

        std::vector<sigma::graphics::static_mesh_data::triangle> submesh_triangles(aimesh->mNumFaces);
        for (unsigned int j = 0; j < aimesh->mNumFaces; ++j) {
            aiFace f = aimesh->mFaces[j];
            for (unsigned int k = 0; k < 3; ++k)
                submesh_triangles[j][k] = f.mIndices[k] + static_cast<unsigned int>(mesh.vertices.size());
        }

        std::string material_name = get_name(aiScene->mMaterials[aimesh->mMaterialIndex]);

        // TODO warn if material slot has been used.
        mesh.materials[resource::identifier{ "material", material_name }.name()] = std::make_pair(mesh.triangles.size(), aimesh->mNumFaces);

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

    entity["sigma::transform"]["position"] = json::to_json(convert_3d(aiposition));
    entity["sigma::transform"]["rotation"] = json::to_json(glm::degrees(glm::eulerAngles(convert_3d(airotation))));
    entity["sigma::transform"]["scale"] = json::to_json(convert_3d(aiscale));

    if (ainode->mNumMeshes > 0) {
        // TODO warn if more than one mesh per object
        const aiMesh* aimesh = aiScene->mMeshes[ainode->mMeshes[0]];
        sigma::resource::identifier meshid("static_mesh", source_file_, get_name(aimesh));
        entity["sigma::graphics::static_mesh"] = meshid.nice_name();
    }

    if (ainode->mMetaData)
        std::cout << ainode->mMetaData->mNumProperties << '\n';

    for (unsigned int i = 0; i < aiScene->mNumLights; ++i) {
        const aiLight* ailight = aiScene->mLights[i];
        if (name == ailight->mName.C_Str()) {
            auto color = convert_color(ailight->mColorDiffuse);
            float intensity = color[0]; // TOOD check intensity and color
            color /= intensity;

            switch (ailight->mType) {
            case aiLightSource_POINT:
                entity["sigma::graphics::point_light"]["color"] = json::to_json(color);
                entity["sigma::graphics::point_light"]["intensity"] = json::to_json(intensity);
                break;
            case aiLightSource_DIRECTIONAL:
                entity["sigma::graphics::directional_light"]["color"] = json::to_json(color);
                entity["sigma::graphics::directional_light"]["intensity"] = json::to_json(intensity);
                break;
            case aiLightSource_SPOT:
                entity["sigma::graphics::spot_light"]["color"] = json::to_json(color);
                entity["sigma::graphics::spot_light"]["intensity"] = json::to_json(intensity);
                entity["sigam::graphics::spot_light"]["cutoff"] = json::to_json(ailight->mAngleOuterCone);
                break;
            default: // TODO more lights
                break;
            }
            break;
        }
    }
}
}

/*
void convert(aiMaterial *aiMat) {
    aiString name;
    aiColor3D diffuse;
    aiColor3D specular;
    aiColor3D ambient;
    aiColor3D emissive;
    aiColor3D transparent;
    int wireframe = 0;
    int twosided = 0;
    int shading_model = 0;
    int blend_func = 0;
    float opacity = 0;
    float shininess = 0;
    float shininess_strength = 0;
    float ior = 0;
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_NAME, name)) {
        //std::cout << "name := "<< name.C_Str() << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)) {
        //std::cout << "diffuse := "<< diffuse.r << " " << diffuse.g << " " <<
diffuse.b << " "  << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_SPECULAR, specular)) {
        //std::cout << "specular := "<< specular.r << " " << specular.g << " "
<< specular.b << " "  << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient)) {
        //std::cout << "ambient := "<< ambient.r << " " << ambient.g << " " <<
ambient.b << " "  << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive)) {
        //std::cout << "emissive := "<< emissive.r << " " << emissive.g << " "
<< emissive.b << " "  << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent)) {
        //std::cout << "transparent := "<< transparent.r << " " << transparent.g
<< " " << transparent.b << " "  << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_ENABLE_WIREFRAME, wireframe)) {
        //std::cout << "wireframe := "<< wireframe << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_TWOSIDED, twosided)) {
        //std::cout << "twosided := "<< twosided << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_SHADING_MODEL, shading_model)) {
        //std::cout << "shading_model := "<< shading_model << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_BLEND_FUNC, blend_func)) {
        //std::cout << "blend_func := "<< blend_func << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_OPACITY, opacity)) {
        //std::cout << "opacity := "<< opacity << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_SHININESS, shininess)) {
        //std::cout << "shininess := "<< shininess << '\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_SHININESS_STRENGTH,
shininess_strength)) {
        //std::cout << "shininess_strength := "<< shininess_strength <<
'\n';
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_REFRACTI, ior)) {
        //std::cout << "ior := "<< ior << '\n';
    }

    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_NONE);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_NONE, n, &texture);
        //std::cout << "none_texture(" << n << ") := " << texture.C_Str() <<
'\n';
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_DIFFUSE);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_DIFFUSE, n, &texture);
        //std::cout << "diffuse_texture(" << n << ") := " << texture.C_Str() <<
'\n';
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_SPECULAR);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_SPECULAR, n, &texture);
        //std::cout << "specular_texture(" << n << ") := " << texture.C_Str() <<
'\n';
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_AMBIENT);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_AMBIENT, n, &texture);
        //std::cout << "ambient_texture(" << n << ") := " << texture.C_Str() <<
'\n';
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_EMISSIVE);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_EMISSIVE, n, &texture);
        //std::cout << "emissive_texture(" << n << ") := " << texture.C_Str() <<
'\n';
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_HEIGHT);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_HEIGHT, n, &texture);
        //std::cout << "height_texture(" << n << ") := " << texture.C_Str() <<
'\n';
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_NORMALS);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_NORMALS, n, &texture);
        //std::cout << "normal_texture(" << n << ") := " << texture.C_Str() <<
'\n';
    }
    for (unsigned int n = 0; n <
aiMat->GetTextureCount(aiTextureType_SHININESS); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_SHININESS, n, &texture);
        //std::cout << "shininess_texture(" << n << ") := " << texture.C_Str()
<< '\n';
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_OPACITY);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_OPACITY, n, &texture);
        //std::cout << "opacity_texture(" << n << ") := " << texture.C_Str() <<
'\n';
    }
    for (unsigned int n = 0; n <
aiMat->GetTextureCount(aiTextureType_DISPLACEMENT); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_DISPLACEMENT, n, &texture);
        //std::cout << "displacement_texture(" << n << ") := " <<
texture.C_Str() << '\n';
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_LIGHTMAP);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_LIGHTMAP, n, &texture);
        //std::cout << "lightmap_texture(" << n << ") := " << texture.C_Str() <<
'\n';
    }
    for (unsigned int n = 0; n <
aiMat->GetTextureCount(aiTextureType_REFLECTION); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_REFLECTION, n, &texture);
        //std::cout << "reflection_texture(" << n << ") := " << texture.C_Str()
<< '\n';
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_UNKNOWN);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_UNKNOWN, n, &texture);
        //std::cout << "unknown_texture(" << n << ") := " << texture.C_Str() <<
'\n';
    }

//    TEXTURE(t,n);
//    TEXBLEND(t,n);
//    TEXOP(t,n);
//    MAPPING(t,n);
//    UVWSRC(t,n);
//    MAPPINGMODE_U(t,n);
//    MAPPINGMODE_V(t,n);
//    TEXMAP_AXIS(t,n);
//    TEXFLAGS(t,n);
}
*/

#include "assimp_converter.hpp"

#include <iostream>
#include <unordered_map>

#include <boost/algorithm/string/predicate.hpp>

#include <glm/gtc/quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <sigma/graphics/static_mesh.hpp>

namespace sigma {
namespace convert {
    namespace {
        std::string get_name(const aiMesh* mesh)
        {
            std::string name = mesh->mName.C_Str();
            if (name == "")
                return "unnamed";
            return name;
        }

        glm::vec3 convert(aiVector3D v) { return glm::vec3(v.x, v.y, v.z); }

        glm::vec2 convert(aiVector2D v) { return glm::vec2(v.x, v.y); }

        glm::quat convert(aiQuaternion q) { return glm::quat(q.x, q.y, q.z, q.w); }
    }

    assimp_converter::assimp_converter(boost::filesystem::path source_file)
        : importer_(std::make_unique<Assimp::Importer>())
    {
        auto filename = source_file.string();
        const aiScene* scene = importer_->ReadFile(
            filename.c_str(),
            aiProcess_CalcTangentSpace | aiProcess_JoinIdenticalVertices |
                // aiProcess_MakeLeftHanded |
                aiProcess_Triangulate |
                // aiProcess_RemoveComponent |
                // aiProcess_GenNormals |
                //???aiProcess_GenSmoothNormals |
                // aiProcess_SplitLargeMeshes |
                // aiProcess_PreTransformVertices |
                aiProcess_LimitBoneWeights | aiProcess_ValidateDataStructure | aiProcess_ImproveCacheLocality | aiProcess_RemoveRedundantMaterials |
                //##aiProcess_FixInfacingNormals | //???
                aiProcess_SortByPType | aiProcess_FindDegenerates | aiProcess_FindInvalidData | aiProcess_GenUVCoords |
                // aiProcess_TransformUVCoords | //???
                aiProcess_FindInstances //|
            // aiProcess_ConvertToLeftHanded|
            // aiProcess_OptimizeMeshes  |
            // aiProcess_OptimizeGraph  |
            // aiProcess_FlipUVs |
            // aiProcess_FlipWindingOrder  |
            // aiProcess_SplitByBoneCount  |
            // aiProcess_Debone //???
            );
        if (scene == nullptr) {
            // TODO error handling
            std::cout << importer_->GetErrorString() << std::endl;
            return;
        }

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
            std::string name = get_name(scene->mMeshes[i]);
            if (!scene->mMeshes[i]->HasBones())
                static_mesh_names_.insert(name);
            else
                static_mesh_names_.erase(name);
        }

        // TODO skeletal mesehes.
        // TODO materials
        // TODO the whole scene.
        // TODO animations.
        // TODO textures?
    }

    assimp_converter::~assimp_converter() {}

    const std::set<std::string>& assimp_converter::static_mesh_names() const
    {
        return static_mesh_names_;
    }

    void assimp_converter::convert_static_mesh(std::string name,
        graphics::static_mesh& mesh) const
    {
        const aiScene* aiScene = importer_->GetScene();

        std::vector<graphics::static_mesh::vertex> vertices;
        std::vector<sigma::graphics::static_mesh::triangle> triangles;
        for (unsigned int i = 0; i < aiScene->mNumMeshes; ++i) {
            const aiMesh* aimesh = aiScene->mMeshes[i];
            if (name != get_name(aimesh))
                continue;

            auto mat = aiScene->mMaterials[aimesh->mMaterialIndex];

            aiString matName;
            mat->Get(AI_MATKEY_NAME, matName);
            std::string material_name = matName.C_Str();
            if (!boost::starts_with(material_name, "material://"))
                material_name = "material://" + material_name;
            mesh.material = material_name;

            std::vector<sigma::graphics::static_mesh::vertex> submesh_vertices(aimesh->mNumVertices);
            for (unsigned int j = 0; j < aimesh->mNumVertices; ++j) {
                auto pos = aimesh->mVertices[j];
                submesh_vertices[j].position = convert(pos);

                if (aimesh->HasNormals()) {
                    auto nor = aimesh->mNormals[j];
                    submesh_vertices[j].normal = convert(nor);
                }

                if (aimesh->HasTangentsAndBitangents()) {
                    auto tan = aimesh->mTangents[j];
                    submesh_vertices[j].tangent = convert(tan);
                }
                if (aimesh->HasTextureCoords(0)) {
                    auto tex = convert(aimesh->mTextureCoords[0][j]);
                    submesh_vertices[j].texcoord = glm::vec2(tex.x, tex.y); // TODO HERE
                }
            }

            std::vector<sigma::graphics::static_mesh::triangle> submesh_triangles(aimesh->mNumFaces);
            for (unsigned int j = 0; j < aimesh->mNumFaces; ++j) {
                aiFace f = aimesh->mFaces[j];
                for (unsigned int k = 0; k < 3; ++k)
                    submesh_triangles[j][k] = f.mIndices[k] + static_cast<unsigned int>(vertices.size());
            }

            vertices.reserve(vertices.size() + aimesh->mNumVertices);
            vertices.insert(vertices.end(), submesh_vertices.begin(), submesh_vertices.end());

            triangles.reserve(triangles.size() + submesh_triangles.size());
            triangles.insert(triangles.end(), submesh_triangles.begin(), submesh_triangles.end());
        }
        // TODO materials
        mesh.vertices = std::move(vertices);
        mesh.triangles = std::move(triangles);
    }
}
}

/*
 *
 * glm::vec3 convert(aiVector3D v) {
    return glm::vec3(v.x, v.y, v.z);
}

glm::vec2 convert(aiVector2D v) {
    return glm::vec2(v.x, v.y);
}

glm::quat convert(aiQuaternion q) {
    return glm::quat(q.x, q.y, q.z, q.w);
}

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
        //std::cout << "name := "<< name.C_Str() << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse)) {
        //std::cout << "diffuse := "<< diffuse.r << " " << diffuse.g << " " <<
diffuse.b << " "  << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_SPECULAR, specular)) {
        //std::cout << "specular := "<< specular.r << " " << specular.g << " "
<< specular.b << " "  << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient)) {
        //std::cout << "ambient := "<< ambient.r << " " << ambient.g << " " <<
ambient.b << " "  << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive)) {
        //std::cout << "emissive := "<< emissive.r << " " << emissive.g << " "
<< emissive.b << " "  << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent)) {
        //std::cout << "transparent := "<< transparent.r << " " << transparent.g
<< " " << transparent.b << " "  << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_ENABLE_WIREFRAME, wireframe)) {
        //std::cout << "wireframe := "<< wireframe << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_TWOSIDED, twosided)) {
        //std::cout << "twosided := "<< twosided << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_SHADING_MODEL, shading_model)) {
        //std::cout << "shading_model := "<< shading_model << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_BLEND_FUNC, blend_func)) {
        //std::cout << "blend_func := "<< blend_func << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_OPACITY, opacity)) {
        //std::cout << "opacity := "<< opacity << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_SHININESS, shininess)) {
        //std::cout << "shininess := "<< shininess << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_SHININESS_STRENGTH,
shininess_strength)) {
        //std::cout << "shininess_strength := "<< shininess_strength <<
std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_REFRACTI, ior)) {
        //std::cout << "ior := "<< ior << std::endl;
    }

    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_NONE);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_NONE, n, &texture);
        //std::cout << "none_texture(" << n << ") := " << texture.C_Str() <<
std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_DIFFUSE);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_DIFFUSE, n, &texture);
        //std::cout << "diffuse_texture(" << n << ") := " << texture.C_Str() <<
std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_SPECULAR);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_SPECULAR, n, &texture);
        //std::cout << "specular_texture(" << n << ") := " << texture.C_Str() <<
std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_AMBIENT);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_AMBIENT, n, &texture);
        //std::cout << "ambient_texture(" << n << ") := " << texture.C_Str() <<
std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_EMISSIVE);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_EMISSIVE, n, &texture);
        //std::cout << "emissive_texture(" << n << ") := " << texture.C_Str() <<
std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_HEIGHT);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_HEIGHT, n, &texture);
        //std::cout << "height_texture(" << n << ") := " << texture.C_Str() <<
std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_NORMALS);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_NORMALS, n, &texture);
        //std::cout << "normal_texture(" << n << ") := " << texture.C_Str() <<
std::endl;
    }
    for (unsigned int n = 0; n <
aiMat->GetTextureCount(aiTextureType_SHININESS); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_SHININESS, n, &texture);
        //std::cout << "shininess_texture(" << n << ") := " << texture.C_Str()
<< std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_OPACITY);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_OPACITY, n, &texture);
        //std::cout << "opacity_texture(" << n << ") := " << texture.C_Str() <<
std::endl;
    }
    for (unsigned int n = 0; n <
aiMat->GetTextureCount(aiTextureType_DISPLACEMENT); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_DISPLACEMENT, n, &texture);
        //std::cout << "displacement_texture(" << n << ") := " <<
texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_LIGHTMAP);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_LIGHTMAP, n, &texture);
        //std::cout << "lightmap_texture(" << n << ") := " << texture.C_Str() <<
std::endl;
    }
    for (unsigned int n = 0; n <
aiMat->GetTextureCount(aiTextureType_REFLECTION); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_REFLECTION, n, &texture);
        //std::cout << "reflection_texture(" << n << ") := " << texture.C_Str()
<< std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_UNKNOWN);
++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_UNKNOWN, n, &texture);
        //std::cout << "unknown_texture(" << n << ") := " << texture.C_Str() <<
std::endl;
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

void assimp_import::import_file(sigma::entity::entity_manager
*entity_manager_,
                                sigma::entity::component_manager
*component_manager_,
                                boost::filesystem::path filepath) {

    auto filename = filepath.string();
    auto path = filepath.replace_extension("");

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename.c_str(),
aiProcess_CalcTangentSpace |
                                                               aiProcess_JoinIdenticalVertices
|
                                                               //aiProcess_MakeLeftHanded
|
                                                               aiProcess_Triangulate
|
                                                               //aiProcess_RemoveComponent
|
                                                               //???aiProcess_GenNormals
|
                                                               //???aiProcess_GenSmoothNormals
|
                                                               //aiProcess_SplitLargeMeshes
|
                                                               //aiProcess_PreTransformVertices
|
                                                               aiProcess_LimitBoneWeights
|
                                                               aiProcess_ValidateDataStructure
|
                                                               aiProcess_ImproveCacheLocality
|
                                                               //??aiProcess_RemoveRedundantMaterials
|
                                                               aiProcess_FixInfacingNormals
| //???
                                                               aiProcess_SortByPType
|
                                                               aiProcess_FindDegenerates
|
                                                               aiProcess_FindInvalidData
|
                                                               aiProcess_GenUVCoords
|
                                                               //aiProcess_TransformUVCoords
| //???
                                                               aiProcess_FindInstances
|
                                                               //aiProcess_ConvertToLeftHanded|
                                                               //aiProcess_OptimizeMeshes
|
                                                               //aiProcess_OptimizeGraph
|
                                                               //aiProcess_FlipUVs
|
                                                               //aiProcess_FlipWindingOrder
|
                                                               //aiProcess_SplitByBoneCount
|
                                                               aiProcess_Debone
//???
    );
    if (scene == nullptr) {
        std::cout << importer.GetErrorString() << std::endl;
        return;
    }


    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        auto aiMat = scene->mMaterials[i];
        convert(aiMat);
    }

    std::unordered_map<int, std::shared_ptr<sigma::graphics::static_mesh>>
static_meshes;
    std::unordered_map<std::string,int> names;
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        auto mesh = scene->mMeshes[i];
        assert(mesh->HasPositions());
        //assert(mesh->HasNormals());
        //assert(mesh->HasTextureCoords(0));
        //assert(mesh->HasTangentsAndBitangents());

        std::vector<sigma::graphics::static_mesh::vertex>
vertices(mesh->mNumVertices);
        std::vector<sigma::graphics::static_mesh::triangle>
triangles(mesh->mNumFaces);

        for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
            auto pos = mesh->mVertices[j];
            vertices[j].position = convert(pos);

            if (mesh->HasNormals()) {
                auto nor = mesh->mNormals[j];
                vertices[j].normal = convert(nor);
            }

            if (mesh->HasTangentsAndBitangents()) {
                auto tan = mesh->mTangents[j];
                vertices[j].tangent = convert(tan);
            }
            if (mesh->HasTextureCoords(0)) {
                auto tex = mesh->mTextureCoords[0][j];
                vertices[j].texcoord = convert(tex);
            }
        }

        for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
            aiFace f = mesh->mFaces[j];
            for (unsigned int k = 0; k < 3; ++k) {
                triangles[j][k] = f.mIndices[k];
            }
        }

        std::string name = mesh->mName.C_Str();
        if(names[mesh->mName.C_Str()] != 0)
            name += "_"+std::to_string(names[mesh->mName.C_Str()]);

        names[mesh->mName.C_Str()]++;

        static_meshes[i] =
std::make_shared<sigma::graphics::static_mesh>(sigma::resource::identifier{path/name});
        static_meshes[i]->set_data(vertices, triangles);
    }

    for (unsigned int i = 0; i < scene->mRootNode->mNumChildren; ++i) {
        aiNode *node = scene->mRootNode->mChildren[i];
        if (node->mNumMeshes <= 0)
            continue;
        auto e = entity_manager_->create();

        aiVector3D scaling;
        aiQuaternion rotation;
        aiVector3D position;
        node->mTransformation.Decompose(scaling, rotation, position);

        auto tc =
component_manager_->add_component<sigma::game::transform_component>(e);
        tc->rotation = convert(rotation);

        tc->position = convert(position);
        tc->scale = convert(scaling);

        auto smc =
component_manager_->add_component<sigma::game::static_mesh_component>(e);
        smc->set_static_mesh(static_meshes[node->mMeshes[0]]);
    }
}

*/

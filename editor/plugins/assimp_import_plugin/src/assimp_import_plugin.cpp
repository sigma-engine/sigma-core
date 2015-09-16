#include <assimp_import_plugin.hpp>

#include <sigmafive/engine.hpp>

#include <sigmafive/graphics/static_mesh.hpp>

#include <sigmafive/game/transform_component.hpp>
#include <sigmafive/game/static_mesh_component.hpp>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <qqml.h>

float3 convert(aiVector3D v) {
    return float3(v.x, v.y, v.z);
}

float2 convert(aiVector2D v) {
    return float2(v.x, v.y);
}

quaternionf convert(aiQuaternion q) {
    return quaternionf(q.x, q.y, q.z, q.w);
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
        //std::cout << "diffuse := "<< diffuse.r << " " << diffuse.g << " " << diffuse.b << " "  << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_SPECULAR, specular)) {
        //std::cout << "specular := "<< specular.r << " " << specular.g << " " << specular.b << " "  << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_AMBIENT, ambient)) {
        //std::cout << "ambient := "<< ambient.r << " " << ambient.g << " " << ambient.b << " "  << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_EMISSIVE, emissive)) {
        //std::cout << "emissive := "<< emissive.r << " " << emissive.g << " " << emissive.b << " "  << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_COLOR_TRANSPARENT, transparent)) {
        //std::cout << "transparent := "<< transparent.r << " " << transparent.g << " " << transparent.b << " "  << std::endl;
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
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_SHININESS_STRENGTH, shininess_strength)) {
        //std::cout << "shininess_strength := "<< shininess_strength << std::endl;
    }
    if (AI_SUCCESS == aiMat->Get(AI_MATKEY_REFRACTI, ior)) {
        //std::cout << "ior := "<< ior << std::endl;
    }

    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_NONE); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_NONE, n, &texture);
        //std::cout << "none_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_DIFFUSE); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_DIFFUSE, n, &texture);
        //std::cout << "diffuse_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_SPECULAR); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_SPECULAR, n, &texture);
        //std::cout << "specular_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_AMBIENT); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_AMBIENT, n, &texture);
        //std::cout << "ambient_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_EMISSIVE); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_EMISSIVE, n, &texture);
        //std::cout << "emissive_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_HEIGHT); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_HEIGHT, n, &texture);
        //std::cout << "height_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_NORMALS); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_NORMALS, n, &texture);
        //std::cout << "normal_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_SHININESS); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_SHININESS, n, &texture);
        //std::cout << "shininess_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_OPACITY); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_OPACITY, n, &texture);
        //std::cout << "opacity_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_DISPLACEMENT); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_DISPLACEMENT, n, &texture);
        //std::cout << "displacement_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_LIGHTMAP); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_LIGHTMAP, n, &texture);
        //std::cout << "lightmap_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_REFLECTION); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_REFLECTION, n, &texture);
        //std::cout << "reflection_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    for (unsigned int n = 0; n < aiMat->GetTextureCount(aiTextureType_UNKNOWN); ++n) {
        aiString texture;
        aiMat->GetTexture(aiTextureType_UNKNOWN, n, &texture);
        //std::cout << "unknown_texture(" << n << ") := " << texture.C_Str() << std::endl;
    }
    /*
    TEXTURE(t,n);
    TEXBLEND(t,n);
    TEXOP(t,n);
    MAPPING(t,n);
    UVWSRC(t,n);
    MAPPINGMODE_U(t,n);
    MAPPINGMODE_V(t,n);
    TEXMAP_AXIS(t,n);
    TEXFLAGS(t,n);*/
}

assimp_import_plugin::assimp_import_plugin(QObject *parent)
        : QObject(parent) {
}


void assimp_import_plugin::import_file(sigmafive::editor::entity_manager_model *model, QString filepath) {
    auto entity_manager_ = model->entity_manager();
    auto component_manager_ = model->component_manager();
    auto resource_manager_ = model->resource_manager();

    std::string filename = QUrl(filepath).toLocalFile().toStdString();
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filename.c_str(), aiProcess_CalcTangentSpace |
                                                               aiProcess_JoinIdenticalVertices |
                                                               //aiProcess_MakeLeftHanded |
                                                               aiProcess_Triangulate |
                                                               //aiProcess_RemoveComponent |
                                                               //???aiProcess_GenNormals |
                                                               //???aiProcess_GenSmoothNormals |
                                                               //aiProcess_SplitLargeMeshes |
                                                               //aiProcess_PreTransformVertices |
                                                               aiProcess_LimitBoneWeights |
                                                               aiProcess_ValidateDataStructure |
                                                               aiProcess_ImproveCacheLocality |
                                                               //??aiProcess_RemoveRedundantMaterials |
                                                               aiProcess_FixInfacingNormals | //???
                                                               aiProcess_SortByPType |
                                                               aiProcess_FindDegenerates |
                                                               aiProcess_FindInvalidData |
                                                               aiProcess_GenUVCoords |
                                                               //aiProcess_TransformUVCoords | //???
                                                               aiProcess_FindInstances |
                                                               //aiProcess_ConvertToLeftHanded|
                                                               //aiProcess_OptimizeMeshes  |
                                                               //aiProcess_OptimizeGraph  |
                                                               //aiProcess_FlipUVs |
                                                               //aiProcess_FlipWindingOrder  |
                                                               //aiProcess_SplitByBoneCount  |
                                                               aiProcess_Debone //???
    );
    if (scene == nullptr) {
        return;
    }

    std::unordered_map<int, boost::uuids::uuid> meshuuids;
    for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
        auto aiMat = scene->mMaterials[i];
        convert(aiMat);
    }
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        auto mesh = scene->mMeshes[i];
        assert(mesh->HasPositions());
        //assert(mesh->HasNormals());
        //assert(mesh->HasTextureCoords(0));
        //assert(mesh->HasTangentsAndBitangents());

        std::vector<sigmafive::graphics::static_mesh::vertex> vertices(mesh->mNumVertices);
        std::vector<sigmafive::graphics::static_mesh::triangle> triangles(mesh->mNumFaces);

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

        auto static_mesh_uuid = resource_manager_->generate_key();
        std::shared_ptr<sigmafive::graphics::static_mesh> static_mesh(new sigmafive::graphics::static_mesh());
        static_mesh->set_data(vertices, triangles);
        resource_manager_->insert(static_mesh_uuid, std::move(static_mesh));
        meshuuids[i] = static_mesh_uuid;
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

        auto tc = component_manager_->add_component<sigmafive::game::transform_component>(e);
        tc->rotation = convert(rotation);

        tc->position = convert(position);
        tc->scale = convert(scaling);

        auto smc = component_manager_->add_component<sigmafive::game::static_mesh_component>(e);
        smc->static_mesh = meshuuids[node->mMeshes[0]];
    }
    emit model->layoutChanged();
}

extern "C" void ASSIMP_IMPORT_PLUGIN_API register_plugin(sigmafive::engine *engine) {
    qmlRegisterType<assimp_import_plugin>("Assimp", 1, 0, "AssimpImporter");
}

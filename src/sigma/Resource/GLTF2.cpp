#include <sigma/Resource/GLTF2.hpp>

#include <fstream>
#include <iostream>

// https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#objects

class GLTF2JsonHelper
{
public:
    GLTF2JsonHelper(const nlohmann::json &inJson)
        : mJson(inJson)
    {
    }

    template <class T>
    T required(const std::string &inKey)
    {
        if (mJson.count(inKey))
        {
            return mJson[inKey];
        }
        throw GLTF2Exception("Missing required key \"" + inKey + "\"");
        return T();
    }

    template <class T>
    T optional(const std::string &inKey, const T &inDefault = T())
    {
        if (mJson.count(inKey))
            return mJson[inKey];
        return inDefault;
    }

private:
    const nlohmann::json &mJson;
};

GLTF2Exception::GLTF2Exception(const std::string &inMsg)
    : mMsg(inMsg)
{
}

const char *GLTF2Exception::what() const noexcept
{
    return mMsg.c_str();
}

void GLTF2Extra::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);
    mName = json.optional<std::string>("name");
    mExtensions = json.optional<nlohmann::json>("extensions");
    mExtras = json.optional<nlohmann::json>("extras");
}

void GLTF2Buffer::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);

    mURI = inDocument.directory() / json.optional<std::string>("uri");

    std::ifstream bufferFile(mURI, std::ios::binary);
    if (!bufferFile.good())
        throw GLTF2Exception("Missing buffer URI \"" + mURI.string() + "\"");

    mBuffer.resize(json.required<size_t>("byteLength"));

    bufferFile.read((char *)mBuffer.data(), mBuffer.size());

    if (!bufferFile.good())
        throw GLTF2Exception("Failed to read buffer from URI \"" + mURI.string() + "\"");

    GLTF2Extra::deserialize(inDocument, inJson);
}

void GLTF2BufferView::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);
    const auto &buffers = inDocument.buffers();

    mBufferIndex = json.required<size_t>("buffer");
    mByteOffset = json.optional<size_t>("byteOffset", 0);
    mByteLength = json.required<size_t>("byteLength");
    mByteStride = json.optional<size_t>("byteStride", 0);
    mTarget = json.optional<size_t>("target", 0);

    if (mBufferIndex >= 0 && mBufferIndex < buffers.size())
    {
        mBuffer = buffers[mBufferIndex];
    }
    else
    {
        throw GLTF2Exception("Buffer " + std::to_string(mBufferIndex) + " doesn't exist!");
    }

    GLTF2Extra::deserialize(inDocument, inJson);
}

void GLTF2Accessor::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);
    const auto &bufferViews = inDocument.bufferViews();

    mBufferViewIndex = json.required<size_t>("bufferView");
    mByteOffset = json.optional<size_t>("byteOffset", 0);
    mComponentType = json.required<size_t>("componentType");
    mNormalized = json.optional<bool>("normalized", 0);
    mCount = json.required<size_t>("count");
    mType = json.required<std::string>("type");

    if (mBufferViewIndex >= 0 && mBufferViewIndex < bufferViews.size())
    {
        mBufferView = bufferViews[mBufferViewIndex];
    }
    else
    {
        throw GLTF2Exception("BufferView " + std::to_string(mBufferViewIndex) + " doesn't exist!");
    }

    GLTF2Extra::deserialize(inDocument, inJson);
}

void GLTF2Image::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);

    if (inJson.count("bufferView"))
    {
        const auto &bufferViews = inDocument.bufferViews();

        mBufferViewIndex = json.required<size_t>("bufferView");

        if (mBufferViewIndex >= 0 && mBufferViewIndex < bufferViews.size())
        {
            mBufferView = bufferViews[mBufferViewIndex];
        }
        else
        {
            throw GLTF2Exception("BufferView " + std::to_string(mBufferViewIndex) + " doesn't exist!");
        }
    }
    else
    {
        mURI = inDocument.directory() / json.required<std::string>("uri");
    }

    mMimeType = json.optional<std::string>("mimeType");

    GLTF2Extra::deserialize(inDocument, inJson);
}
void GLTF2Sampler::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);

    mMagFilter = json.optional("magFilter", LINEAR);
    mMinFilter = json.optional("minFilter", LINEAR);

    mWrapS = json.optional("wrapS", REPEAT);
    mWrapT = json.optional("wrapT", REPEAT);

    GLTF2Extra::deserialize(inDocument, inJson);
}

void GLTF2Texture::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);
    const auto &samplers = inDocument.samplers();
    const auto &images = inDocument.images();

    if (inJson.count("sampler"))
    {
        mSamplerIndex = json.required<size_t>("sampler");
        if (mSamplerIndex >= 0 && mSamplerIndex < samplers.size())
        {
            mSampler = samplers[mSamplerIndex];
        }
        else
        {
            throw GLTF2Exception("Sampler " + std::to_string(mSamplerIndex) + " doesn't exist!");
        }
    }

    mSourceIndex = json.required<size_t>("source");
    if (mSourceIndex >= 0 && mSourceIndex < images.size())
    {
        mImage = images[mSamplerIndex];
    }
    else
    {
        throw GLTF2Exception("Image " + std::to_string(mSourceIndex) + " doesn't exist!");
    }

    GLTF2Extra::deserialize(inDocument, inJson);
}

void GLTF2TextureInfo::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);
    const auto &textures = inDocument.textures();

    mTextureIndex = json.required<size_t>("index");
    if (mTextureIndex >= 0 && mTextureIndex < textures.size())
    {
        mTexture = textures[mTextureIndex];
    }
    else
    {
        throw GLTF2Exception("Texture " + std::to_string(mTextureIndex) + " doesn't exist!");
    }

    mTexCoord = json.optional<size_t>("texCoord", 0);

    GLTF2Extra::deserialize(inDocument, inJson);
}

void GLTF2Material::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);

    if (inJson.count("pbrMetallicRoughness"))
    {
        const nlohmann::json &inPbr = inJson["pbrMetallicRoughness"];
        GLTF2JsonHelper pbrJson(inPbr);
        mBaseColorFactor = pbrJson.optional<std::array<float, 4>>("baseColorFactor", {1, 1, 1, 1});

        if (inPbr.count("baseColorTexture"))
        {
            mBaseColorTexture = std::make_shared<GLTF2TextureInfo>();
            mBaseColorTexture->deserialize(inDocument, inPbr["baseColorTexture"]);
        }

        mMetallicFactor = pbrJson.optional<float>("metallicFactor", 1);
        mRoughnessFactor = pbrJson.optional<float>("roughnessFactor", 1);

        if (inPbr.count("metallicRoughnessTexture"))
        {
            mMetallicRoughnessTexture = std::make_shared<GLTF2TextureInfo>();
            mMetallicRoughnessTexture->deserialize(inDocument, inPbr["metallicRoughnessTexture"]);
        }
    }

    if (inJson.count("normalTexture"))
    {
        mNormalTexture = std::make_shared<GLTF2TextureInfo>();
        mNormalTexture->deserialize(inDocument, inJson["normalTexture"]);
    }

    if (inJson.count("occlusionTexture"))
    {
        mOcclusionTexture = std::make_shared<GLTF2TextureInfo>();
        mOcclusionTexture->deserialize(inDocument, inJson["occlusionTexture"]);
    }

    if (inJson.count("emissiveTexture"))
    {
        mEmissiveTexture = std::make_shared<GLTF2TextureInfo>();
        mEmissiveTexture->deserialize(inDocument, inJson["emissiveTexture"]);
    }

    mEmissiveFactor = json.optional<std::array<float, 3>>("emissiveFactor", {0, 0, 0});
    mAlphaMode = json.optional<std::string>("alphaMode", "OPAQUE");
    mAlphaCutoff = json.optional<float>("alphaCutoff", 0.5);
    mDoubleSided = json.optional<bool>("doubleSided", false);

    GLTF2Extra::deserialize(inDocument, inJson);
}

void GLTF2Primitive::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);
    const auto &accessors = inDocument.accessors();
    const auto &materials = inDocument.materials();

    mAttributeAccessorIndices = json.required<std::unordered_map<std::string, size_t>>("attributes");
    for (const auto &accessorInfo : mAttributeAccessorIndices)
    {
        if (accessorInfo.second >= 0 && accessorInfo.second < accessors.size())
        {
            mAttributeAccessors[accessorInfo.first] = accessors[accessorInfo.second];
        }
        else
        {
            throw GLTF2Exception("Accessor " + std::to_string(accessorInfo.second) + " doesn't exist!");
        }
    }

    if (inJson.count("indices"))
    {
        mIndicesAccessorIndex = json.required<size_t>("indices");
        if (mIndicesAccessorIndex >= 0 && mIndicesAccessorIndex < accessors.size())
        {
            mIndicesAccessor = accessors[mIndicesAccessorIndex];
        }
        else
        {
            throw GLTF2Exception("Accessor " + std::to_string(mIndicesAccessorIndex) + " doesn't exist!");
        }
    }

    if (inJson.count("material"))
    {
        mMaterialIndex = json.required<size_t>("material");
        if (mMaterialIndex >= 0 && mMaterialIndex < materials.size())
        {
            mMaterial = materials[mMaterialIndex];
        }
        else
        {
            throw GLTF2Exception("Material " + std::to_string(mMaterialIndex) + " doesn't exist!");
        }
    }

    mMode = json.optional<size_t>("mode", TRIANGLES);

    GLTF2Extra::deserialize(inDocument, inJson);
}

void GLTF2Mesh::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    GLTF2JsonHelper json(inJson);

    if (inJson.count("primitives"))
    {
        for (const auto &jPrimitive : inJson["primitives"])
        {
            GLTF2Primitive primitive;
            primitive.deserialize(inDocument, jPrimitive);
            mPrimitives.push_back(std::move(primitive));
        }
    }
    else
    {
        throw GLTF2Exception("Missing required key \"primitives\"");
    }

    mWeights = json.optional<std::vector<float>>("weights");

    GLTF2Extra::deserialize(inDocument, inJson);
}

GLTF2Document::GLTF2Document(const std::filesystem::path &inDirectory)
    : mDirectory(inDirectory)
{
}

const std::filesystem::path &GLTF2Document::directory() const
{
    return mDirectory;
}

void GLTF2Document::deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson)
{
    if (inJson.count("buffers"))
    {
        for (auto jBuffer : inJson["buffers"])
        {
            auto buffer = std::make_shared<GLTF2Buffer>();
            buffer->deserialize(*this, jBuffer);
            mBuffers.push_back(buffer);
        }
    }

    if (inJson.count("bufferViews"))
    {
        for (auto jBufferView : inJson["bufferViews"])
        {
            auto bufferView = std::make_shared<GLTF2BufferView>();
            bufferView->deserialize(*this, jBufferView);
            mBufferViews.push_back(bufferView);
        }
    }

    if (inJson.count("accessors"))
    {
        for (auto jAccessor : inJson["accessors"])
        {
            auto accessor = std::make_shared<GLTF2Accessor>();
            accessor->deserialize(*this, jAccessor);
            mAccessors.push_back(accessor);
        }
    }

    if (inJson.count("images"))
    {
        for (auto jImage : inJson["images"])
        {
            auto image = std::make_shared<GLTF2Image>();
            image->deserialize(*this, jImage);
            mImages.push_back(image);
        }
    }

    if (inJson.count("samplers"))
    {
        for (auto jSampler : inJson["samplers"])
        {
            auto sampler = std::make_shared<GLTF2Sampler>();
            sampler->deserialize(*this, jSampler);
            mSamplers.push_back(sampler);
        }
    }

    if (inJson.count("textures"))
    {
        for (auto jTexture : inJson["textures"])
        {
            auto texture = std::make_shared<GLTF2Texture>();
            texture->deserialize(*this, jTexture);
            mTextures.push_back(texture);
        }
    }

    if (inJson.count("materials"))
    {
        for (auto jMaterail : inJson["materials"])
        {
            auto material = std::make_shared<GLTF2Material>();
            material->deserialize(*this, jMaterail);
            mMaterials.push_back(material);
        }
    }

    if (inJson.count("meshes"))
    {
        for (auto jMeshes : inJson["meshes"])
        {
            auto mesh = std::make_shared<GLTF2Mesh>();
            mesh->deserialize(*this, jMeshes);
            mMeshes.push_back(mesh);
        }
    }

    GLTF2Extra::deserialize(inDocument, inJson);
}
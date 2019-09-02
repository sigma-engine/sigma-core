#pragma once

#include <nlohmann/json.hpp>

#include <filesystem>
#include <exception>
#include <string>

class GLTF2Document;

class GLTF2Exception : public std::exception
{
public:
    GLTF2Exception(const std::string &inMsg);

    virtual const char *what() const noexcept override;

private:
    std::string mMsg;
};

class GLTF2Extra
{
public:
    virtual ~GLTF2Extra() = default;

    const std::string &name() const { return mName; }

    const nlohmann::json &extensions() const { return mExtensions; }

    const nlohmann::json &extras() const { return mExtras; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson);

private:
    std::string mName;
    nlohmann::json mExtensions;
    nlohmann::json mExtras;
};

class GLTF2Buffer : public GLTF2Extra
{
public:
    const std::filesystem::path &uri() const { return mURI; }

    size_t byteLength() const { return mBuffer.size(); }

    const uint8_t *data() const { return mBuffer.data(); }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

private:
    std::filesystem::path mURI;
    std::vector<uint8_t> mBuffer;
};

class GLTF2BufferView : public GLTF2Extra
{
public:
    std::shared_ptr<GLTF2Buffer> buffer() const { return mBuffer; }

    size_t bufferIndex() const { return mBufferIndex; }

    size_t byteOffset() const { return mByteOffset; }

    size_t byteLength() const { return mByteLength; }

    size_t byteStride() const { return mByteStride; }

    size_t target() const { return mTarget; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

private:
    size_t mBufferIndex = 0;
    size_t mByteOffset = 0;
    size_t mByteLength = 0;
    size_t mByteStride = 0;
    size_t mTarget = 0;

    std::shared_ptr<GLTF2Buffer> mBuffer = nullptr;
};

class GLTF2Accessor : public GLTF2Extra
{
public:
    std::shared_ptr<GLTF2BufferView> bufferView() const { return mBufferView; }

    size_t bufferViewIndex() const { return mBufferViewIndex; }

    size_t byteOffset() const { return mByteOffset; }

    size_t componentType() const { return mComponentType; }

    bool normalized() const { return mNormalized; }

    size_t count() const { return mCount; }

    std::string type() const { return mType; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

private:
    size_t mBufferViewIndex = 0;
    size_t mByteOffset = 0;
    size_t mComponentType = 0;
    bool mNormalized = false;
    size_t mCount = 0;
    std::string mType;

    std::shared_ptr<GLTF2BufferView> mBufferView = nullptr;
};

class GLTF2Image : public GLTF2Extra
{
public:
    std::filesystem::path uri() const { return mURI; }

    std::string mimeType() const { return mMimeType; }

    size_t bufferViewIndex() const { return mBufferViewIndex; }

    std::shared_ptr<GLTF2BufferView> bufferView() const { return mBufferView; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

private:
    std::filesystem::path mURI;
    std::string mMimeType;
    size_t mBufferViewIndex = 0;

    std::shared_ptr<GLTF2BufferView> mBufferView = nullptr;
};

class GLTF2Sampler : public GLTF2Extra
{
public:
    static constexpr const size_t NEAREST = 9728;
    static constexpr const size_t LINEAR = 9729;
    static constexpr const size_t NEAREST_MIPMAP_NEAREST = 9984;
    static constexpr const size_t LINEAR_MIPMAP_NEAREST = 9985;
    static constexpr const size_t NEAREST_MIPMAP_LINEAR = 9986;
    static constexpr const size_t LINEAR_MIPMAP_LINEAR = 9987;

    static constexpr const size_t CLAMP_TO_EDGE = 33071;
    static constexpr const size_t MIRRORED_REPEAT = 33648;
    static constexpr const size_t REPEAT = 10497;

    size_t magfilter() const { return mMagFilter; }

    size_t minfilter() const { return mMinFilter; }

    size_t wrapS() const { return mWrapS; }

    size_t wrapT() const { return mWrapT; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

private:
    size_t mMagFilter;
    size_t mMinFilter;
    size_t mWrapS;
    size_t mWrapT;
};

class GLTF2Texture : public GLTF2Extra
{
public:
    size_t samplerIndex() const { return mSamplerIndex; }

    size_t sourceIndex() const { return mSourceIndex; }

    std::shared_ptr<GLTF2Sampler> sampler() const { return mSampler; }

    std::shared_ptr<GLTF2Image> image() const { return mImage; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

private:
    size_t mSamplerIndex = 0;
    size_t mSourceIndex = 0;

    std::shared_ptr<GLTF2Sampler> mSampler = nullptr;
    std::shared_ptr<GLTF2Image> mImage = nullptr;
};

class GLTF2TextureInfo : public GLTF2Extra
{
public:
    size_t textureIndex() const { return mTextureIndex; }

    size_t texCoord() const { return mTexCoord; }

    std::shared_ptr<GLTF2Texture> texture() const { return mTexture; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

private:
    size_t mTextureIndex = 0;
    size_t mTexCoord = 0;

    std::shared_ptr<GLTF2Texture> mTexture = nullptr;
};

class GLTF2Material : public GLTF2Extra
{
public:
    std::array<float, 4> baseColorFactor() const { return mBaseColorFactor; }

    std::shared_ptr<GLTF2TextureInfo> baseColorTexture() const { return mBaseColorTexture; }

    float metallicFactor() const { return mMetallicFactor; }

    float roughnessFactor() const { return mRoughnessFactor; }

    std::shared_ptr<GLTF2TextureInfo> metallicRoughnessTexture() const { return mMetallicRoughnessTexture; }

    std::shared_ptr<GLTF2TextureInfo> normalTexture() const { return mNormalTexture; }

    std::shared_ptr<GLTF2TextureInfo> occlusionTexture() const { return mOcclusionTexture; }

    std::shared_ptr<GLTF2TextureInfo> emissiveTexture() const { return mEmissiveTexture; }

    std::array<float, 3> emissiveFactor() const { return mEmissiveFactor; }

    std::string alphaMode() const { return mAlphaMode; }

    float alphaCutoff() const { return mAlphaCutoff; }

    bool doubleSided() const { return mDoubleSided; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

private:
    std::array<float, 4> mBaseColorFactor = {1, 1, 1, 1};
    std::shared_ptr<GLTF2TextureInfo> mBaseColorTexture;
    float mMetallicFactor = 1;
    float mRoughnessFactor = 1;
    std::shared_ptr<GLTF2TextureInfo> mMetallicRoughnessTexture = nullptr;
    std::shared_ptr<GLTF2TextureInfo> mNormalTexture = nullptr;
    std::shared_ptr<GLTF2TextureInfo> mOcclusionTexture = nullptr;
    std::shared_ptr<GLTF2TextureInfo> mEmissiveTexture = nullptr;
    std::array<float, 3> mEmissiveFactor = {0, 0, 0};
    std::string mAlphaMode = "OPAQUE";
    float mAlphaCutoff = 0.5;
    bool mDoubleSided = false;
};

class GLTF2Primitive : public GLTF2Extra
{
public:
    static constexpr const size_t POINTS = 0;
    static constexpr const size_t LINES = 1;
    static constexpr const size_t LINE_LOOP = 2;
    static constexpr const size_t LINE_STRIP = 3;
    static constexpr const size_t TRIANGLES = 4;
    static constexpr const size_t TRIANGLE_STRIP = 5;
    static constexpr const size_t TRIANGLE_FAN = 6;

    size_t indicesAccessorIndex() const { return mIndicesAccessorIndex; }

    size_t materialIndex() const { return mMaterialIndex; }

    size_t mode() const { return mMode; }

    const std::unordered_map<std::string, std::shared_ptr<GLTF2Accessor>> &attributeAccessors() const { return mAttributeAccessors; }

    std::shared_ptr<GLTF2Accessor> indicesAccessor() const { return mIndicesAccessor; }

    std::shared_ptr<GLTF2Material> material() const { return mMaterial; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

private:
    std::unordered_map<std::string, size_t> mAttributeAccessorIndices;
    size_t mIndicesAccessorIndex = 0;
    size_t mMaterialIndex = 0;
    size_t mMode = TRIANGLES;

    std::unordered_map<std::string, std::shared_ptr<GLTF2Accessor>> mAttributeAccessors;
    std::shared_ptr<GLTF2Accessor> mIndicesAccessor = nullptr;
    std::shared_ptr<GLTF2Material> mMaterial = nullptr;
};

class GLTF2Mesh : public GLTF2Extra
{
public:
    const std::vector<GLTF2Primitive> &primitives() const { return mPrimitives; }

    const std::vector<float> &weights() const { return mWeights; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

private:
    std::vector<GLTF2Primitive> mPrimitives;
    std::vector<float> mWeights;
};

class GLTF2Document : public GLTF2Extra
{
public:
    GLTF2Document(const std::filesystem::path &inDirectory);

    const std::filesystem::path &directory() const;

    const std::vector<std::shared_ptr<GLTF2Buffer>> &buffers() const { return mBuffers; }

    const std::vector<std::shared_ptr<GLTF2BufferView>> &bufferViews() const { return mBufferViews; }

    const std::vector<std::shared_ptr<GLTF2Accessor>> &accessors() const { return mAccessors; }

    const std::vector<std::shared_ptr<GLTF2Image>> &images() const { return mImages; }

    const std::vector<std::shared_ptr<GLTF2Sampler>> &samplers() const { return mSamplers; }

    const std::vector<std::shared_ptr<GLTF2Texture>> &textures() const { return mTextures; }

    const std::vector<std::shared_ptr<GLTF2Material>> &materials() const { return mMaterials; }

    const std::vector<std::shared_ptr<GLTF2Mesh>> &meshes() const { return mMeshes; }

    virtual void deserialize(const GLTF2Document &inDocument, const nlohmann::json &inJson) override;

public:
    std::filesystem::path mDirectory;

    std::vector<std::shared_ptr<GLTF2Buffer>> mBuffers;
    std::vector<std::shared_ptr<GLTF2BufferView>> mBufferViews;
    std::vector<std::shared_ptr<GLTF2Accessor>> mAccessors;
    std::vector<std::shared_ptr<GLTF2Image>> mImages;
    std::vector<std::shared_ptr<GLTF2Sampler>> mSamplers;
    std::vector<std::shared_ptr<GLTF2Texture>> mTextures;
    std::vector<std::shared_ptr<GLTF2Material>> mMaterials;
    std::vector<std::shared_ptr<GLTF2Mesh>> mMeshes;
};

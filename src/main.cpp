#include <sigma/Algorithm.hpp>

#include <SimpleGame.hpp>

#include <string>
#include <vector>


/* namespace gltf {
using Buffer = std::vector<uint8_t>;

struct BufferView {
    uint64_t buffer;
    uint64_t byteOffset;
    uint64_t byteSize;
    uint64_t byteStride;
    uint64_t target;
};

struct BufferAccessor {
    uint64_t bufferView;
    uint64_t byteOffset;
    uint64_t componentType;
    uint64_t count;
    uint64_t type;
};

class Mesh {
};

struct Scene {
    std::vector<Buffer> buffers;
    std::vector<BufferView> bufferViews;
    std::vector<BufferAccessor> accessors;
    std::vector<Mesh> meshes;
};
}

void loadRegistry(const std::filesystem::path& inPath, entt::registry& inRegistry)
{
    const auto& folder = inPath.parent_path();
    std::ifstream file(inPath);
    const auto& gltfJson = nlohmann::json::parse(file);
    const auto& gltfJsonBuffers = gltfJson["buffers"];
    const auto& gltfJsonBufferViews = gltfJson["bufferViews"];
    const auto& gltfJsonMeshes = gltfJson["meshes"];
    gltf::Scene scene;
    scene.buffers.resize(gltfJsonBuffers.size());
    scene.bufferViews.resize(gltfJsonBufferViews.size());
    scene.meshes.resize(gltfJsonMeshes.size());

    for (size_t i = 0; i < scene.buffers.size(); ++i) {
        auto bufferPath = folder / std::string(gltfJsonBuffers[i]["uri"]);
        std::ifstream bufferFile(bufferPath, std::ios::binary);
        scene.buffers[i].resize(gltfJsonBuffers[i]["byteLength"]);
        bufferFile.read(reinterpret_cast<char*>(scene.buffers[i].data()), scene.buffers[i].size());
    }

    for (size_t i = 0; i < scene.bufferViews.size(); ++i) {
        scene.bufferViews[i].buffer = gltfJsonBufferViews[i]["buffer"];
        scene.bufferViews[i].byteOffset = gltfJsonBufferViews[i]["byteOffset"];
        scene.bufferViews[i].byteSize = gltfJsonBufferViews[i]["byteLength"];
        if (gltfJsonBufferViews[i].count("byteStride"))
            scene.bufferViews[i].byteStride = gltfJsonBufferViews[i]["byteStride"];
        scene.bufferViews[i].target = gltfJsonBufferViews[i]["target"];
    }
}
*/

int main(int argc, char const* argv[])
{
    std::vector<std::string> arguments(argc - 1);
    std::transform(argv + 1, argv + (argc - 1) + 1, arguments.begin(), [](auto arg) { return arg; });
    std::stable_sort(arguments.begin(), arguments.end(), [](auto a, auto b) {
        return StartsWith(a, "-") && !StartsWith(b, "-");
    });

    SimpleGame game;
    return game.run(arguments);
}

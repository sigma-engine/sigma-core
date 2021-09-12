#include <sigma/Tools/SceneConverter.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

void SceneConverter::convert(const std::filesystem::path& path)
{
	Assimp::Importer importer;
	unsigned int	 flags = aiProcess_CalcTangentSpace
		| aiProcess_GenNormals
		| aiProcess_JoinIdenticalVertices
		| aiProcess_Triangulate
		| aiProcess_GenUVCoords
		| aiProcess_SortByPType
		| aiProcess_ImproveCacheLocality
		| aiProcess_LimitBoneWeights
		| aiProcess_ValidateDataStructure
		| aiProcess_FindInvalidData
		| aiProcess_TransformUVCoords
		| aiProcess_Debone;
	const aiScene* scene = importer.ReadFile(path.string(), flags);
}

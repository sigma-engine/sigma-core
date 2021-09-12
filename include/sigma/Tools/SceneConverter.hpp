#pragma once

#include <filesystem>

class SceneConverter
{
public:
	/**
	 * @brief Convert a 3D model into a sigma scene.
	 * @param path the path to the 3D model to convert.
	*/
	void convert(const std::filesystem::path& path);
};
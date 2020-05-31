#pragma once

#include <string>
#include <vector>
#include <memory>

class SimpleCameraController;

class SimpleGame {
public:
	int run(const std::vector<std::string>& inArguments);

private:
	std::shared_ptr<SimpleCameraController> mCameraController = nullptr;
};
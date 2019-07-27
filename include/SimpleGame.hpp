#pragma once

#include <entt/entt.hpp>

#include <string>
#include <vector>

class SimpleCameraController;

class SimpleGame {
public:
    int run(const std::vector<std::string>& inArguments);

private:
    entt::registry mRegistry;
    std::shared_ptr<SimpleCameraController> mCameraController = nullptr;
};
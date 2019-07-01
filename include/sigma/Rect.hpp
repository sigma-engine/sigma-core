#pragma once

#include <glm/vec2.hpp>

template <class T>
struct Rect {
    glm::vec<2, T> origin;
    glm::vec<2, T> size;
};

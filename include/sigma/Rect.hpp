#pragma once

#include <glm/vec2.hpp>
#include <type_traits>

template <class T>
struct Rect {
	glm::vec<2, T> origin;
	glm::vec<2, std::make_unsigned_t<T>> size;
};

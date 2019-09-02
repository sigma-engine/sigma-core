#pragma once

#include <algorithm>

template <class T, class U>
bool StartsWith(const T& inContainer, const U& inPattern)
{
    auto ic = std::begin(inContainer);
    auto ec = std::end(inContainer);

    auto ip = std::begin(inPattern);
    auto ep = std::end(inPattern);

    while (ic != ec && ip != ep && *ic == *ip) {
        ic++;
        ip++;
    }

    return ip == ep;
}

template<typename T>
void HashCombineHelper(size_t& seed, const T& val)
{
	seed ^= std::hash<T>()(val) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template<typename... Types>
size_t HashCombine(const Types&... args)
{
	size_t seed = 0;
	(HashCombineHelper(seed, args), ...);
	return seed;
}
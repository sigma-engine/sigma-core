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
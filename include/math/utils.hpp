#ifndef UTILS_HPP
#define UTILS_HPP

#include <cmath>
#include <limits>
#include <algorithm>
#include <type_traits>

template<class T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type almost_equal(T x, T y,unsigned int ulp = 2) {
    return std::abs(x-y) < std::numeric_limits<T>::epsilon() * std::abs(x+y) * ulp
        // unless the result is subnormal
               || std::abs(x-y) < std::numeric_limits<T>::min();
}

template<class T>
typename std::enable_if<std::numeric_limits<T>::is_integer, bool>::type almost_equal(T a, T b) {
	return a==b;
}

template<class T>
T clamp(T inValue,T min,T max) {
    return std::min(std::max(inValue,min),max);
}

template<class T>
T remap(T inValue,T inLow,T inHi,T outLow,T outHi) {
    return (((inValue - inLow) * (outHi - outLow)) / (inHi - inLow)) + outLow;
}

static long long nCr(int n, int r) {
    if(r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long p = 1;
    for(int i = 1; i <= r; i++) {
        p *= n - r + i;
        p /= i;
    }
    return p;
}

#endif //UTILS_HPP

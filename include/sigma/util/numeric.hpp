#ifndef SIGMA_UTIL_NUMERIC_HPP
#define SIGMA_UTIL_NUMERIC_HPP

namespace sigma {
namespace numeric {
    template <typename T>
    static constexpr T round_up(T number, T multiple)
    {
        return ((number + multiple - 1) / multiple) * multiple;
    }
}
}

#endif // SIGMA_UTIL_NUMERIC_HPP

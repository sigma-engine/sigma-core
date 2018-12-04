#ifndef SIGMA_ENGINE_STRING_HPP
#define SIGMA_ENGINE_STRING_HPP

#include <iterator>
#include <locale>

namespace sigma {
namespace util {

    template <class String>
    String to_upper_copy(String str)
    {
        std::locale loc;
        std::transform(str.begin(), str.end(), str.begin(), [&loc](auto c) {
            return std::toupper(c, loc);
        });
        return str;
    }

    template <class ContainerA, class ContainerB>
    bool starts_with(const ContainerA& a, const ContainerB& b)
    {
        auto a_len = std::distance(a.begin(), a.end());
        auto b_len = std::distance(b.begin(), b.end());

        if (a_len < b_len)
            return false;

        return std::equal(b.begin(), b.end(), a.begin());
    }

    template <class ContainerA, class ContainerB>
    bool ends_with(const ContainerA& a, const ContainerB& b)
    {
        auto a_begin = a.begin();
        auto a_len = std::distance(a.begin(), a.end());
        auto b_len = std::distance(b.begin(), b.end());

        if (a_len < b_len)
            return false;

        std::advance(a_begin, (a_len - b_len));
        return std::equal(b.begin(), b.end(), a_begin);
    }
}
}

#endif // SIGMA_ENGINE_STRING_HPP

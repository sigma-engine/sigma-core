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
        std::transform(std::begin(str), std::end(str), std::begin(str), [&loc](auto c) {
            return std::toupper(c, loc);
        });
        return str;
    }

    template <class String>
    String to_lower_copy(String str)
    {
        std::locale loc;
        std::transform(std::begin(str), std::end(str), std::begin(str), [&loc](auto c) {
            return std::tolower(c, loc);
        });
        return str;
    }

    template <class ContainerA, class ContainerB>
    bool starts_with(const ContainerA& a, const ContainerB& b)
    {
        auto a_len = std::distance(std::begin(a), std::end(a));
        auto b_len = std::distance(std::begin(b), std::end(b));

        if (a_len < b_len)
            return false;

        return std::equal(std::begin(b), std::end(b), std::begin(a));
    }

    template <class ContainerA, class ContainerB>
    bool ends_with(const ContainerA& a, const ContainerB& b)
    {
        auto a_begin = std::begin(a);
        auto a_len = std::distance(std::begin(a), std::end(a));
        auto b_len = std::distance(std::begin(b), std::end(b));

        if (a_len < b_len)
            return false;

        std::advance(a_begin, (a_len - b_len - 1));
        return std::equal(std::begin(b), std::end(b), a_begin);
    }

    template<class String>
    String replace_all(String str, const String& old_str, const String& new_str)
    {
        size_t start = 0;
        while ((start = str.find(old_str, start)) != String::npos) {
            str.replace(start, old_str.length(), new_str);
            start += new_str.length();
        }
        return str;
    }
}
}

#endif // SIGMA_ENGINE_STRING_HPP

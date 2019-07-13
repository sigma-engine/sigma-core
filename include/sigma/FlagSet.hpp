#pragma once

#include <type_traits>

template <class T>
class FlagSet {
public:
    FlagSet(const T& inValue)
        : mValue(static_cast<value_type>(inValue))
    {
    }

    FlagSet<T> operator|(const T& inValue) const
    {
        return FlagSet(static_cast<T>(static_cast<value_type>(mValue) | static_cast<value_type>(inValue)));
    }

    FlagSet<T> operator|(const FlagSet<T>& inValue) const
    {
        return FlagSet(static_cast<T>(static_cast<value_type>(mValue) | static_cast<value_type>(inValue.mValue)));
    }

    FlagSet<T> operator&(const T& inValue) const
    {
        return FlagSet(static_cast<T>(static_cast<value_type>(mValue) & static_cast<value_type>(inValue)));
    }

    FlagSet<T> operator&(const FlagSet<T>& inValue) const
    {
        return FlagSet(static_cast<T>(static_cast<value_type>(mValue) & static_cast<value_type>(inValue.mValue)));
    }

    bool isSet(const T& inValue) const
    {
        return (static_cast<value_type>(mValue) & static_cast<value_type>(inValue)) == static_cast<value_type>(inValue);
    }

    bool isSet(const FlagSet<T>& inValue) const
    {
        return (static_cast<value_type>(mValue) & static_cast<value_type>(inValue.mValue)) == inValue.mValue;
    }

private:
    using value_type = std::underlying_type_t<T>;
    value_type mValue;
};
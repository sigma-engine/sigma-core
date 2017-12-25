#ifndef SIGMA_REFLECT_HPP
#define SIGMA_REFLECT_HPP

#include <sigma/util/json_conversion.hpp>
#include <sigma/util/std140_conversion.hpp>

#include <boost/hana/adapt_struct.hpp>
#include <boost/hana/define_struct.hpp>

#ifdef __REFLECTION_PARSER__
#define R_EXPORT(...) __attribute__((annotate("R_EXPORT," #__VA_ARGS__)))
#define R_FIELD(...) __attribute__((annotate(#__VA_ARGS__)))
#else
#define R_EXPORT(...)
#define R_FIELD(...)
#endif

#endif // SIGMA_REFLECT_HPP

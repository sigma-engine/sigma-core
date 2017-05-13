#ifndef SIGMA_REFLECT_HPP
#define SIGMA_REFLECT_HPP

#ifdef __REFLECTION_PARSER__
#define R_ENUM(...) __attribute__((annotate("R_ENUM")))
#define R_COMPONENT(...) __attribute__((annotate("R_CLASS")))
#define R_FIELD(...) __attribute__((annotate(#__VA_ARGS__)))
#else
#define R_ENUM(...)
#define R_COMPONENT(...) __VA_ARGS__
#define R_FIELD(...)
#endif

#endif // SIGMA_REFLECT_HPP

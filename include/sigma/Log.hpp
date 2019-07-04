#pragma once

#include <cassert>
#include <spdlog/spdlog.h>

#define SIGMA_LOG_NAME "SIGMA"
#define SIGMA_TRACE(...) spdlog::get(SIGMA_LOG_NAME)->trace(__VA_ARGS__);
#define SIGMA_DEBUG(...) spdlog::get(SIGMA_LOG_NAME)->debug(__VA_ARGS__);
#define SIGMA_INFO(...) spdlog::get(SIGMA_LOG_NAME)->info(__VA_ARGS__);
#define SIGMA_WARN(...) spdlog::get(SIGMA_LOG_NAME)->warn(__VA_ARGS__);
#define SIGMA_ERROR(...) spdlog::get(SIGMA_LOG_NAME)->error(__VA_ARGS__);
#define SIGMA_CRITICAL(...) spdlog::get(SIGMA_LOG_NAME)->critical(__VA_ARGS__);

// clang-format off
#ifdef NDEBUG
#define SIGMA_ASSERT(condition, ...)
#else
#define SIGMA_ASSERT(condition, ...) if(!(condition)) { spdlog::get(SIGMA_LOG_NAME)->critical("{0}:{1}: In {2}\n                                             {0}:{1}: {3}", __FILE__, __LINE__, __PRETTY_FUNCTION__, fmt::format(__VA_ARGS__)); std::abort(); }
#endif
// clang-format on
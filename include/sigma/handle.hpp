#ifndef SIGMA_HANDLE_HPP
#define SIGMA_HANDLE_HPP

#include <sigma/config.hpp>

#include <cstddef>
#include <cstdint>
#include <functional>
#include <vector>

namespace sigma {

struct handle {
    std::uint32_t index;
    std::uint32_t version;

    handle();

    handle(std::uint32_t index, std::uint32_t version);

    bool operator==(handle o) const noexcept;

    bool operator!=(handle o) const noexcept;

    bool is_valid() const;
};

handle SIGMA_API create_handle(std::vector<handle>& handles, std::vector<std::uint32_t>& free_handles);

bool SIGMA_API is_valid_handle(const std::vector<handle>& handles, const std::vector<std::uint32_t>& free_handles, handle hnd);

void SIGMA_API destroy_handle(std::vector<handle>& handles, std::vector<std::uint32_t>& free_handles, handle hnd);
}

namespace std {
template <>
struct SIGMA_API hash<sigma::handle> {
    size_t operator()(const sigma::handle& h) const;
};
}

#endif // SIGMA_HANDLE_HPP

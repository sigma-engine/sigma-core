#include <sigma/handle.hpp>

#include <boost/functional/hash.hpp>

namespace sigma {

handle::handle()
    : index(std::uint32_t(-1))
    , version(std::uint32_t(-1))
{
}

handle::handle(std::uint32_t index, std::uint32_t version)
    : index(index)
    , version(version)
{
}

bool handle::operator==(handle o) const noexcept
{
    return index == o.index && version == o.version;
}

bool handle::operator!=(handle o) const noexcept
{
    return index != o.index || version != o.version;
}

bool handle::is_valid() const
{
    return index != std::uint32_t(-1) && version != std::uint32_t(-1);
}

handle create_handle(std::vector<handle>& handles, std::vector<std::uint32_t>& free_handles)
{
    if (free_handles.empty()) {
        handle e{ std::uint32_t(handles.size()), 0 };
        handles.push_back(e);
        return e;
    } else {
        auto index = free_handles.back();
        free_handles.pop_back();
        handles[index].index = index;
        handles[index].version++;
        return handles[index];
    }
}

bool is_valid_handle(const std::vector<handle>& handles, const std::vector<std::uint32_t>& free_handles, handle hnd)
{
    return hnd.index < handles.size() && handles[hnd.index].index != std::uint32_t(-1) && handles[hnd.index].version == hnd.version;
}

void destroy_handle(std::vector<handle>& handles, std::vector<std::uint32_t>& free_handles, handle hnd)
{
    if (!is_valid_handle(handles, free_handles, hnd))
        return;
    free_handles.push_back(handles[hnd.index].index);
    handles[hnd.index].index = std::uint32_t(-1);
}
}

std::size_t std::hash<sigma::handle>::operator()(const sigma::handle& h) const
{
    std::size_t seed = 0;
    boost::hash_combine(seed, h.index);
    boost::hash_combine(seed, h.version);
    return seed;
}

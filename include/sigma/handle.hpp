#ifndef SIGMA_HANDLE_HPP
#define SIGMA_HANDLE_HPP

#include <sigma/config.hpp>

#include <boost/functional/hash.hpp>

#include <cstddef>
#include <cstdint>
#include <vector>

namespace sigma {

template <class TagType>
struct handle {
    std::uint32_t index;
    std::uint32_t version;

    handle()
        : index(std::uint32_t(-1))
        , version(std::uint32_t(-1))
    {
    }

    handle(std::uint32_t index, std::uint32_t version)
        : index(index)
        , version(version)
    {
    }

    bool operator==(handle<TagType> o) const noexcept
    {
        return index == o.index && version == o.version;
    }

    bool operator!=(handle<TagType> o) const noexcept
    {
        return index != o.index || version != o.version;
    }

    bool is_valid() const noexcept
    {
        return index != std::uint32_t(-1) && version != std::uint32_t(-1);
    }

    handle<TagType> bump_version() const noexcept
    {
        if (version == std::uint32_t(-1))
            return { index, 0 };
        return { index, version };
    }

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
        ar & this->index;
        ar & this->version;
    }
};

template <class TagType>
handle<TagType> create_handle(std::vector<handle<TagType>>& handles, std::vector<std::uint32_t>& free_handles)
{
    if (free_handles.empty()) {
        handle<TagType> e{ std::uint32_t(handles.size()), 0 };
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

template <class TagType>
bool is_valid_handle(const std::vector<handle<TagType>>& handles, const std::vector<std::uint32_t>& free_handles, handle<TagType> hnd)
{
    return hnd.index < handles.size() && handles[hnd.index].index != std::uint32_t(-1) && handles[hnd.index].version == hnd.version;
}

template <class TagType>
void destroy_handle(std::vector<handle<TagType>>& handles, std::vector<std::uint32_t>& free_handles, handle<TagType> hnd)
{
    if (!is_valid_handle(handles, free_handles, hnd))
        return;
    free_handles.push_back(handles[hnd.index].index);
    handles[hnd.index].index = std::uint32_t(-1);
}
}

namespace std {
template <class TagType>
struct hash<sigma::handle<TagType>> {
    size_t operator()(const sigma::handle<TagType>& h) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, h.index);
        boost::hash_combine(seed, h.version);
        return seed;
    }
};
}

#endif // SIGMA_HANDLE_HPP

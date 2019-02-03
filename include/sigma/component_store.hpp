#ifndef SIGMA_COMPONENT_STORE_HPP
#define SIGMA_COMPONENT_STORE_HPP

#include <algorithm>
#include <unordered_set>
#include <utility>
#include <vector>

namespace sigma {

template <class LeftContainer, class RightContainer>
class join_container {
public:
    using left_iterator = typename LeftContainer::iterator;
    using left_data_type = typename left_iterator::value_type::second_type;
    using right_iterator = typename LeftContainer::iterator;
    using right_data_type = typename right_iterator::value_type::second_type;

    class iterator {
    public:
        iterator(left_iterator lit, left_iterator lend, right_iterator rit, right_iterator rend)
            : lit_(lit)
            , lend_(lend)
            , rit_(rit)
            , rend_(rend)
        {
        }

    private:
        left_iterator lit_, lend_;
        right_iterator rit_, rend_;
    };

    join_container(LeftContainer& left, RightContainer& right)
        : left_(left)
        , right_(right)
    {
    }

    size_t size() const
    {
        return std::min(left_.size(), right_.size());
    }

    iterator begin()
    {
        auto lit = left_.begin();
        auto rit = right_.end();
        while (lit != left_.end()) {
            rit = std::find(right_.begin(), right_.end(), [lit](const auto& a) {
                return lit->first == a.fist;
            });

            if (rit != right_.end())
                break;
            else
                lit++;
        }
        return iterator { lit, left_.end(), rit, right_.end() };
    }

    iterator end()
    {
        return iterator { left_.end(), left_.end(), right_.end(), right_.end() };
    }

    std::tuple<uint32_t, left_iterator::type::second_type> private : LeftContainer& left_;
    RightContainer& right_;
};

class base_component_store {
public:
    base_component_store() = default;
    base_component_store(const base_component_store&) = delete;
    base_component_store(base_component_store&&) = delete;
    virtual ~base_component_store() = default;
    base_component_store& operator=(const base_component_store&) = delete;
    base_component_store& operator=(base_component_store&&) = delete;
};
template <class T>
class component_store : public base_component_store {
public:
    using iterator = typename std::vector<std::pair<uint32_t, T>>::iterator;

    void commit()
    {
        auto it = std::remove_if(data_.begin(), data_.end(), [this](const auto& a) {
            return remove_.count(a.first) > 0;
        });

        bool added = false;
        for (auto eid : create_) {
            if (!added && it != data_.end()) {
                *it = std::make_pair(eid, T {});
                it++;
            } else {
                added = true;
                data_.emplace_back(eid, T {});
            }
        }

        if (!added)
            data_.erase(it, data_.end());

        remove_.clear();
        create_.clear();

        // TODO: maybe use insertion sort here?
        std::sort(data_.begin(), data_.end(), [](const auto& a, const auto& b) {
            return a.first < b.first;
        });
    }

    void add(uint32_t eid)
    {
        if (data_.size() < data_.capacity())
            data_.emplace_back(eid, T {});
        else
            create_.insert(eid);
    }

    void remove(uint32_t eid)
    {
        remove_.insert(eid);
    }

    const T* get(uint32_t eid) const
    {
        auto it = std::find_if(data_.begin(), data_.end(), [eid](const auto& a) {
            return a.first == eid;
        });

        if (it == data_.end())
            return nullptr;

        return &it->second;
    }

    T* get(uint32_t eid)
    {
        auto it = std::find_if(data_.begin(), data_.end(), [eid](const auto& a) {
            return a.first == eid;
        });

        if (it == data_.end())
            return nullptr;

        return &it->second;
    }

    template <class U>
    join_container<component_store<T>, component_store<U>> join(component_store<U>& other)
    {
        return join_container<component_store<T>, component_store<U>>(*this, other);
    }

private:
    std::unordered_set<uint32_t> remove_;
    std::unordered_set<uint32_t> create_;
    std::vector<std::pair<uint32_t, T>> data_;
};
}
#endif // SIGMA_COMPONENT_STORE_HPP

#ifndef SIGMAFIVE_FACTORY_HPP
#define SIGMAFIVE_FACTORY_HPP

#include <sigmafive/config.hpp>
#include <cppbr/meta/object.hpp>

#include <memory>

namespace sigmafive {
    template<typename Interface>
    class factory : public cppbr::meta::object {
    CPPBR_META_CLASS()
    public:
        virtual ~factory() = default;

        virtual std::unique_ptr<Interface> create() = 0;
    };

    template<class Interface, class Concrete>
    class simple_factory : public factory<Interface> {
    CPPBR_META_CLASS()
    public:
        virtual std::unique_ptr<Interface> create() override {
            return std::unique_ptr<Concrete>(new Concrete{});
        }
    };
}

#endif //SIGMAFIVE_FACTORY_HPP

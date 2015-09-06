#ifndef SIGMAFIVE_FACTORY_HPP
#define SIGMAFIVE_FACTORY_HPP

#include <memory>

namespace sigmafive {
    template<typename Interface>
    class factory {
    public:
        virtual ~factory() = default;

        virtual std::unique_ptr<Interface> create() = 0;
    };

    template<class Interface,class Concrete>
    class simple_factory : public factory<Interface> {
    public:
        virtual std::unique_ptr<Interface> create() override {
            return std::unique_ptr<Concrete>(new Concrete{});
        }
    };
}

#endif //SIGMAFIVE_FACTORY_HPP

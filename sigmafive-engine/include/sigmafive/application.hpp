#ifndef SIGMAFIVE_APPLICATION_HPP
#define SIGMAFIVE_APPLICATION_HPP

#include <sigmafive/context.hpp>

namespace sigmafive {
    class application {
    public:
        application(int &argc, char **argv);

        virtual ~application();

        static application &instance();

        sigmafive::context &context();
    private:
        static application *instance_;

        sigmafive::context context_;
    };
}

#endif //SIGMAFIVE_APPLICATION_HPP

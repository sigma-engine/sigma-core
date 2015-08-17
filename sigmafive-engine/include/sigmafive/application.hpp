#ifndef SIGMAFIVE_APPLICATION_HPP
#define SIGMAFIVE_APPLICATION_HPP

#include <sigmafive/context.hpp>

#include <json/json.h>

namespace sigmafive {
    class application {
    public:
        application(int &argc, char **argv);

        virtual ~application();

        static application &instance();
    private:
        static application *instance_;

        Json::Value properties_;
    };
}

#endif //SIGMAFIVE_APPLICATION_HPP

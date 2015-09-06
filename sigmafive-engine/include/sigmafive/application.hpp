#ifndef SIGMAFIVE_APPLICATION_HPP
#define SIGMAFIVE_APPLICATION_HPP

namespace sigmafive {
    class application {
    public:
        application(int &argc, char **argv);

        virtual ~application();

        static application &instance();
    private:
        static application *instance_;
    };
}

#endif //SIGMAFIVE_APPLICATION_HPP

#include <sigmafive/application.hpp>

#include <stdexcept>

namespace sigmafive {

    application::application(int &argc, char **argv) {
        if(instance_ != nullptr)
            throw std::runtime_error("can not create more than one instance of sigmafive::application.");
        instance_ = this;
    }

    application::~application() {
        instance_ = nullptr;
    }

    application &application::instance() {
        if(instance_ == nullptr)
            throw std::runtime_error("did not create an instance of sigmafive::application.");
        return *instance_;
    }

    application *application::instance_ = nullptr;
}

#include <system/window.hpp>

int main(int argc, char const *argv[]) {
    sigmafive::system::context_attributes context_attributes;
    context_attributes.red = 8;
    context_attributes.green = 8;
    context_attributes.blue = 8;
    context_attributes.alpha = 8;
    context_attributes.depth = 24;
    context_attributes.stencil = 8;
    context_attributes.samples = 0;
    context_attributes.major = 4;
    context_attributes.minor = 4;
    context_attributes.double_buffer = true;
    context_attributes.core_profile = false;
    context_attributes.vsync = false;
    sigmafive::system::window window("Sigma Five", int2(800, 600), context_attributes);

    while(window.good()) {
    }

    return 0;
}

#include <sigma/world.hpp>

#include <gmock/gmock.h>

struct basic_component {
    basic_component(int x, int y)
        : x(x)
        , y(y)
    {
    }

    int x, y;
};

struct construction_component {
    construction_component(int x, int y)
        : x(x)
        , y(y)
    {
    }

    int x, y;
};

struct destruction_component {
    destruction_component(int x, int y)
        : x(x)
        , y(y)
    {
    }

    virtual ~destruction_component()
    {
        destructor();
    }

    MOCK_METHOD0(destructor, void());

    int x, y;
};

using test_components = sigma::component_set<basic_component, construction_component, destruction_component>;
using test_world = sigma::world<test_components>;

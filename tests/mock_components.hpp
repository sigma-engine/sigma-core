#include <sigma/world.hpp>

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

using test_world = sigma::world<basic_component, construction_component, destruction_component>;

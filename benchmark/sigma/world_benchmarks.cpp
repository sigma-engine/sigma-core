#include <benchmark/benchmark.h>

#include <sigma/world.hpp>

//#include "setup.hpp"

using namespace sigma;

struct position {
    position(float x = 0.0f, float y = 0.0f)
        : x(x)
        , y(y)
    {
    }
    float x, y;
};

struct direction {
    direction(float x = 0.0f, float y = 0.0f)
        : x(x)
        , y(y)
    {
    }
    float x, y;
};

using benchmark_components = component_set<position, direction>;
using benchmark_world = world<benchmark_components>;

struct movement_system1 {

    void process(benchmark_world& w, float dt)
    {
        w.for_each<position, direction>([&](entity, auto& pos, const auto& dir) {
            this->inner_loop(pos, dir, dt);
        });
    }

    void inner_loop(position& pos, const direction& dir, float dt) const
    {
        pos.x += dir.x * dt;
        pos.y += dir.y * dt;
    }
};

class entity_system : public benchmark::Fixture {
public:
    entity_system()
    {
    }
};

BENCHMARK_DEFINE_F(entity_system, for_each)
(benchmark::State& st)
{
    benchmark_world w;
    movement_system1 sys1;

    for (int i = 0; i < st.range(0); ++i) {
        auto e = w.create();
        w.add<position>(e, 1.0f, 3.0f);
        w.add<direction>(e, 2.0f, 3.0f);
        w.remove<direction>(e);
    }

    while (st.KeepRunning()) {
        benchmark::DoNotOptimize(&w);
        sys1.process(w, 1.0f / 60.0f);
        // TODO there is a newer version of google benchmark that has
        // support ClobberMemory but it is not in conan yet.
#ifndef _MSC_VER
        benchmark::ClobberMemory();
#endif
    }
    st.SetComplexityN(st.range(0));
}

BENCHMARK_REGISTER_F(entity_system, for_each)
    ->RangeMultiplier(2)
    ->Range(100, 1000000)
    ->Complexity();

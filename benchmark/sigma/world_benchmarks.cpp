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

using my_world = world<position, direction>;

struct movement_system1 {

    void process(my_world& w, double dt)
    {
        w.for_each<position, direction>([&](entity, auto& pos, const auto& dir) {
            this->inner_loop(pos, dir, dt);
        });
    }

    void inner_loop(position& pos, const direction& dir, double dt) const
    {
        pos.x += dir.x * dt;
        pos.y += dir.y * dt;
    }
};

class world_benchmarks : public benchmark::Fixture {
public:
    world_benchmarks()
    {
    }
};

BENCHMARK_DEFINE_F(world_benchmarks, movement_system)
(benchmark::State& st)
{
    my_world w;
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
        benchmark::ClobberMemory();
    }
    st.SetComplexityN(st.range(0));
}

BENCHMARK_REGISTER_F(world_benchmarks, movement_system)
    ->RangeMultiplier(2)
    ->Range(100, 1000000)
    ->Complexity();

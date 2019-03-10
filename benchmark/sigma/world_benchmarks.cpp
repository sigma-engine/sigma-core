#include <benchmark/benchmark.h>

#include <entt/entt.hpp>

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

struct movement_system1 {
    void process(entt::registry<>& registry, float dt)
    {
        registry.group<position, direction>().each([&](const auto, auto &pos, auto &dir) {
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

BENCHMARK_DEFINE_F(entity_system, each)
(benchmark::State& st)
{
    entt::registry<> registry;
    movement_system1 sys1;

    for (int i = 0; i < st.range(0); ++i) {
        auto e = registry.create();
        registry.assign<position>(e, 1.0f, 3.0f);
        if(i % 2 == 0) { 
            registry.assign<direction>(e, 2.0f, 3.0f);
        }
    }

    while (st.KeepRunning()) {
        benchmark::DoNotOptimize(&registry);
        sys1.process(registry, 1.0f / 60.0f);
        // TODO there is a newer version of google benchmark that has
        // support ClobberMemory but it is not in conan yet.
#ifndef _MSC_VER
        benchmark::ClobberMemory();
#endif
    }
    st.SetComplexityN(st.range(0));
}

BENCHMARK_REGISTER_F(entity_system, each)
    ->RangeMultiplier(2)
    ->Range(100, 10000000)
    ->Complexity();
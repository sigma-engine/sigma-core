#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/entity/component.hpp>
#include <sigmafive/entity/default_component_cache.hpp>

using namespace sigmafive::entity;

struct c1 {};

TEST(default_component_cache_tests,when_a_component_has_be_added_should_not_be_nullptr) {
    default_component_cache<c1> pool;

    entity e{0,0};
    pool.add(e);

    EXPECT_NE(nullptr,pool.get(e));
}

// TODO add other test for debug mode?

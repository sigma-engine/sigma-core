#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <sigmafive/entity/entity.hpp>
#include <sigmafive/entity/component.hpp>
#include <sigmafive/entity/component_cache.hpp>

using namespace sigmafive::entity;

using ::testing::_;
using ::testing::Return;

struct c1 {

};

template<class T>
class mock_cache : public component_cache<T> {
public:
    MOCK_METHOD1(add, void(entity));
    MOCK_METHOD1_T(get, component<T>(entity));
    MOCK_METHOD1_T(acquire, T*(entity));
    MOCK_CONST_METHOD1_T(acquire, const T*(entity));
    MOCK_METHOD1(remove, bool(entity));
};

TEST(component_tests,default_construct_should_be_nullptr) {
    component<c1> c;

    EXPECT_TRUE(c == nullptr);
    EXPECT_FALSE(c != nullptr);
    EXPECT_TRUE(nullptr == c);
    EXPECT_FALSE(nullptr != c);
}

TEST(component_tests,nullptr_pool_valid_entity_construct_should_be_nullptr) {
    component<c1> c{entity{0,0},nullptr};

    EXPECT_TRUE(c == nullptr);
    EXPECT_FALSE(c != nullptr);
    EXPECT_TRUE(nullptr == c);
    EXPECT_FALSE(nullptr != c);
}

TEST(component_tests,non_nullptr_pool_and_invalid_entity_construct_should_be_nullptr) {
    mock_cache<c1> pool;

    entity e;
    component<c1> c{e,&pool};

    EXPECT_TRUE(c == nullptr);
    EXPECT_FALSE(c != nullptr);
    EXPECT_TRUE(nullptr == c);
    EXPECT_FALSE(nullptr != c);
}

TEST(component_tests,non_nullptr_pool_and_valid_entity_construct_should_not_be_nullptr) {
    mock_cache<c1> pool;
    c1 cmp;

    entity e{0,0};
    component<c1> c{e,&pool};

    EXPECT_CALL(pool,acquire(e)).Times(4).WillRepeatedly(Return(&cmp));

    EXPECT_FALSE(c == nullptr);
    EXPECT_TRUE(c != nullptr);
    EXPECT_FALSE(nullptr == c);
    EXPECT_TRUE(nullptr != c);
}

TEST(component_tests,should_equal_nullptr_if_pool_returns_nullptr) {
    mock_cache<c1> pool;

    entity e{0,0};
    component<c1> c{e,&pool};

    EXPECT_CALL(pool,acquire(e)).Times(4).WillRepeatedly(Return(nullptr));

    EXPECT_TRUE(c == nullptr);
    EXPECT_FALSE(c != nullptr);

    EXPECT_TRUE(nullptr == c);
    EXPECT_FALSE(nullptr != c);
}

TEST(component_tests,two_components_should_not_be_equal_if_they_have_different_pools_but_same_entities) {
    mock_cache<c1> pool1,pool2;

    entity e{1,1};
    component<c1> cmp1{e,&pool1};
    component<c1> cmp2{e,&pool2};

    EXPECT_FALSE(cmp1 == cmp2);
    EXPECT_TRUE(cmp1 != cmp2);
}

TEST(component_tests,two_components_should_not_be_equal_if_they_have_same_pools_but_different_entities) {
    mock_cache<c1> pool;

    entity e1{1,1};
    component<c1> cmp1{e1,&pool};

    entity e2{2,1};
    component<c1> cmp2{e2,&pool};

    EXPECT_FALSE(cmp1 == cmp2);
    EXPECT_TRUE(cmp1 != cmp2);
}

TEST(component_tests,two_components_should_be_equal_if_they_have_same_pools_and_entities) {
    mock_cache<c1> pool;

    entity e{1,1};
    component<c1> cmp1{e,&pool};
    component<c1> cmp2{e,&pool};

    EXPECT_TRUE(cmp1 == cmp2);
    EXPECT_FALSE(cmp1 != cmp2);
}

TEST(component_tests,pool_does_not_have_component_get_should_return_nullptr) {
    mock_cache<c1> pool;

    entity e{1,1};
    component<c1> c{e,&pool};

    EXPECT_CALL(pool,acquire(e)).Times(1).WillOnce(Return(nullptr));

    EXPECT_TRUE(c == nullptr);
}

TEST(component_tests,pool_has_component_get_should_return_the_component) {
    mock_cache<c1> pool;

    entity e{1,1};
    component<c1> c{e,&pool};

    c1 cmp;
    EXPECT_CALL(pool,acquire(e)).Times(1).WillOnce(Return(&cmp));

    EXPECT_TRUE(c.get() == &cmp);
}
